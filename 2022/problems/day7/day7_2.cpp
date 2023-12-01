#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>

#include <cstdio>
#include <cstdlib>

using namespace std;

// A directory
typedef struct TreeTag {
  string name;
  int size;
  int capacity;
  struct TreeTag **children;

  struct TreeTag *parent;

  // total size
  long total;
} Tree;

static void insert( Tree *tree, string name )
{
  if ( tree->size == tree->capacity ) {
    tree->capacity = ( tree->size + 1 ) * 2;
    tree->children = (Tree **)realloc( tree->children, tree->capacity * sizeof(Tree *) );
  }

  Tree *child = (Tree *)calloc( 1, sizeof( Tree ) );
  child->name = name;
  child->parent = tree;

  tree->children[ tree->size++ ] = child;
}

// It must be there
static Tree *find( Tree *tree, string name ) {
  for( int i = 0; i < tree->size; i++ ) {
    if ( tree->children[ i ]->name == name ) {
      return tree->children[ i ];
    }
  }

  return NULL;
}

vector<long> part2;

// Returns the total of this + sub
static long getTotal( Tree *tree )
{
  long total = tree->total;
  for ( int i = 0; i < tree->size; i++ )
    total += getTotal( tree->children[ i ] );
  return total;
}

static void printTree( Tree *tree )
{
  cout << "name = " << tree->name << endl;
  int total = getTotal( tree );
  cout << "total = " << total << endl;

  part2.push_back( total );

  for ( int i = 0; i < tree->size; i++ )
    printTree( tree->children[ i ] );
}

static void freeTree( Tree *tree )
{
  for ( int i = 0; i < tree->size; i++ )
    freeTree( tree->children[ i ] );

  free( tree );
}

int main()
{
  Tree *current = NULL;
  Tree *root = (Tree *)calloc( 1, sizeof( Tree ) );
  root->name = "/";

  vector<string> lines;
  string line;
  while( getline( cin, line ) ) {
    lines.push_back( line );
  }

  int idx = 0;
  while ( idx < lines.size() ) {
    string line = lines[ idx ];
 
    stringstream ss( line );

    // Remove $
    string dollar;
    ss >> dollar;

    string cmd;
    ss >> cmd;

    if ( cmd == "cd" ) {
      string name;
      ss >> name;
      // Root as a special case
      if ( name == "/" ) {
        current = root;
      } else if ( name == ".." ) {
        current = current->parent;
      } else {
        current = find( current, name );
      }
      idx++;
    } else { // Assume ls
      // Try to peak ahead
      while ( ++idx < lines.size() ) {
        line = lines[ idx ];
        if ( line[ 0 ] == '$' )
          break;
        
        stringstream lsStream( line );
        string first, name;
        lsStream >> first >> name;

        // Either a dir or a file
        if ( first == "dir" ) {
          insert( current, name );
        } else {
          current->total += atoi( first.c_str() );
        }
      }
    }
  }

  printTree( root );
  int diskSpace = 70000000;
  int needed = 30000000;
  sort( part2.begin(), part2.end() );
  int freeSpace = diskSpace - part2.back();
  bool found = false;
  int ans = -1;
  for ( auto size : part2 ) {
    string acceptane = freeSpace + size >= needed ? "YES " : "NO ";
    if ( !found && acceptane == "YES " ) {
      found = true;
      ans = size;
    }
    cout << acceptane << size << endl;
  }
  cout << "Ans = " << ans << endl;

  freeTree( root );
}
