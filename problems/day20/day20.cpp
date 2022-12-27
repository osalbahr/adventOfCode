#define DEBUG

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>

#include <cstdio>
#include <cassert>

using namespace std;

#define REPORT( X ) cout << #X << " = " << (X) << endl

// Aliasing
#define forn( X ) \
for ( int i_ = 0; i_ < ( X ); i_++ )

static void printList( const list<int>& numbers, FILE *fp )
{
  for ( int n : numbers ) {
    fprintf( fp, "%d\n", n );
  }
}

static void printListReverse( const list<int>& numbers, FILE *fp )
{
  for ( auto it = numbers.rbegin(); it != numbers.rend(); it++ )
    fprintf( fp, "%d\n", *it );
}

#ifdef DEBUG
static void printListCommas( list<int> numbers, FILE *fp, const int *n )
{
  if ( n == NULL )
    fprintf( fp, "Initial arrangement:\n" );
  else
    fprintf( fp, "%d moves:\n", *n );
  
  auto node = numbers.begin();
  fprintf( fp, "%d", *node );

  while ( (++node) != numbers.end() )
    fprintf( fp, ", %d", *node );

  fprintf( fp, "\n" );
}
#endif

static void mixNode( list<int>& numbers, list<int>::iterator node )
{
  int n = *node;
  if ( n == 0 )
    return;
  
  int moves = abs( n ) % ( numbers.size() - 1 );

  if ( n > 0 ) {
    forn( moves ) {
      auto pos = numbers.erase( node );
      if ( pos == numbers.end() ) {
        pos = numbers.begin();
      }
      pos++;
      node = numbers.insert( pos, n );
    }
  } else { // n < 0
    forn( moves ) {
      auto pos = numbers.erase( node );
      pos--;
      if ( pos == numbers.begin() ) {
        pos = numbers.end();
      }
      node = numbers.insert( pos, n );
    }
  }
}

static void mix( list<int>& numbers )
{
#ifdef DEBUG
  FILE *out = fopen( "out", "w" );
  printListCommas( numbers, out, NULL );
#endif
  vector< list<int>::iterator > nodes;
  for ( auto node = numbers.begin(); node != numbers.end(); node++ )
    nodes.push_back( node );
  
  for ( auto node : nodes ) {
#ifdef DEBUG
    int n = *node;
#endif
    mixNode( numbers, node );
#ifdef DEBUG
    fprintf( out, "\n" );
    printListCommas( numbers, out, &n );
#endif
  }
#ifdef DEBUG
  fclose( out );
#endif
}

static int getZeroIdx( const list<int>& numbers )
{
  int idx = 0;
  for ( auto it = numbers.begin(); it != numbers.end(); it++ ) {
    if ( *it == 0 )
      return idx;
    idx++;
  }
  cerr << "Zero node not found" << endl;
  exit( 1 );
}

int main()
{
  list<int> numbers;

  int n;
  while ( cin >> n )
    numbers.push_back( n );

  // Check it
  fclose( fopen( "PARSE", "w" ) );
  FILE *parse = fopen( "PARSE", "w" );
  printList( numbers, parse );
  fclose( parse );

  // Check it reversed
  fclose( fopen( "REVERSE", "w" ) );
  FILE *rev = fopen( "REVERSE", "w" );
  printListReverse( numbers, rev );
  fclose( rev );

  // Mix them
  mix( numbers );

  // printListCommas( numbers, stdout, NULL );
  
  int zeroIdx = getZeroIdx( numbers );
  REPORT( zeroIdx );

  // Dump it all in a vector
  vector<int> numbersVec;
  for ( int n : numbers )
    numbersVec.push_back( n );

  assert( numbersVec.size() == numbers.size() );
  int size = numbers.size();

  REPORT( numbersVec[ zeroIdx ] );

  int n1, n2, n3;
  REPORT( ( n1 = numbersVec[ ( zeroIdx + 1000 ) % size ] ) );
  REPORT( ( n2 = numbersVec[ ( zeroIdx + 2000 ) % size ] ) );
  REPORT( ( n3 = numbersVec[ ( zeroIdx + 3000 ) % size ] ) );

  int total = n1 + n2 + n3;
  REPORT( total );

  assert( total == 3 || total == 7225 );
}
