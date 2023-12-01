#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

using namespace std;

// A directory
class Tree {
  public:
    string name;
    // Maps name to child
    map<string, Tree> children;
    Tree *parent = NULL;
    // Total size
    long total = 0;
    Tree( string arg );
};

Tree::Tree( string arg ) {
  name = arg;
}

int main()
{
  Tree *current = NULL;  
  Tree root( "/" );

  vector<string> lines;
  string line;
  while( getline( cin, line ) ) {
    lines.push_back( line );
  }

  int idx = 0;
  while ( idx < lines.size() ) {
    // You shouldn't be here
    if ( line[ 0 ] != '$' ) {
      cerr << idx << " : " << line << endl;
      exit( 1 );
    }

    stringstream ss( lines[ idx ] );

    // Remove $
    string dollar;
    ss >> dollar;

    string cmd;
    ss >> cmd;

    if ( cmd == "cd" ) {
      string name;
      // Root as a special case
      if ( name == "/" ) {
        current = &root;
      } else {
        current = &current->children[ name ];
      }
      idx++;
    } else { // Assume ls
      // Try to peak ahead
      while ( idx < lines.size() ) {
        line = lines[ idx ];
        if ( line[ 0 ] == '$' )
          break;
        
        stringstream lsStream( line );
        string first, name;
        lsStream >> first >> name;

        // Either a dir or a file
        if ( first == "dir" ) {
          Tree child( name );
          
        } else {
          current->total += atoi( first.c_str() );
        }

        idx++;
      }
    }
  }
}
