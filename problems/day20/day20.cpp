#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>

#include <cstdio>

using namespace std;

#define REPORT( X ) cout << #X << " = " << (X) << endl

// Aliasing
#define forn( X ) \
for ( int i_ = 0; i_ < ( X ); i_++ )

static void printList( list<int> numbers, FILE *fp )
{
  for ( int n : numbers ) {
    fprintf( fp, "%d\n", n );
  }
}

static void printListReverse( list<int> numbers, FILE *fp )
{
  for ( auto it = numbers.rbegin(); it != numbers.rend(); it++ )
    fprintf( fp, "%d\n", *it );
}

// static void printListCommas( list<int> numbers, FILE *fp, const int *n )
// {
//   if ( n == NULL )
//     cout << "Initial arrangement:" << endl;
//   else
//     cout << *n << " moves:" << endl;
  
//   auto node = numbers.begin();
//   fprintf( fp, "%d", *node );

//   while ( (++node) != numbers.end() )
//     fprintf( fp, ", %d", *node );

//   cout << endl;
// }

static void mixNode( list<int>& numbers, list<int>::iterator node )
{
  int n = *node;
  if ( n == 0 )
    return;

  if ( n > 0 ) {
    forn( n ) {
      auto pos = numbers.erase( node );
      if ( pos == numbers.end() ) {
        pos = numbers.begin();
      }
      pos++;
      node = numbers.insert( pos, n );
    }
  } else { // n < 0
    forn( abs( n ) ) {
      auto pos = numbers.erase( node );
      pos--;
      if ( pos == numbers.begin() ) {
        pos = numbers.end();
      }
      node = numbers.insert( pos, n );
      // printListCommas( numbers, stdout, &n );
    }
  }
}

static void mix( list<int>& numbers )
{
  // printListCommas( numbers, stdout, NULL );
  // cout << endl;

  vector< list<int>::iterator > nodes;
  for ( auto node = numbers.begin(); node != numbers.end(); node++ )
    nodes.push_back( node );
  
  for ( auto node : nodes ) {
    // int n = *node;
    mixNode( numbers, node );
    // printListCommas( numbers, stdout, &n );
    // cout << endl;
  }

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
  REPORT( n1 + n2 + n3 );
}
