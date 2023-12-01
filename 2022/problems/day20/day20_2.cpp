// #define DEBUG

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
#define forn( X )\
assert( (X) >= 0 );\
for ( size_t i_ = 0; i_ < size_t(X); i_++ )\


int key = 811589153;
#define KEY key
// #define flush ""

int mixCount = 10;

static void printList( const list<long>& numbers, FILE *fp )
{
  for ( long n : numbers ) {
    fprintf( fp, "%ld\n", n );
  }
}

static void printListReverse( const list<long>& numbers, FILE *fp )
{
  for ( auto it = numbers.rbegin(); it != numbers.rend(); it++ )
    fprintf( fp, "%ld\n", *it );
}

#ifdef DEBUG
static void printListCommas( list<long> numbers, FILE *fp, long *n )
{
  if ( n != NULL )
    fprintf( fp, "%ld moves:\n", *n );

  auto node = numbers.begin();
  fprintf( fp, "%ld", *node );

  while ( (++node) != numbers.end() )
    fprintf( fp, ", %ld", *node );

  fprintf( fp, "\n" );
}
#endif

#ifdef DEBUG
FILE *out;
#endif

static list<long>::iterator mixNode( list<long>& numbers, list<long>::iterator node )
{
  long n = *node;
  if ( n == 0 )
    return node;

#ifndef SLOW
  int moves = abs( n ) % ( numbers.size() - 1 );
#else
  long moves = abs( n );
#endif

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
    if ( node == numbers.begin() ) {
      moves++;
      moves %= numbers.size() - 1;
    }
    forn( moves ) {
      auto pos = numbers.erase( node );
      pos--;
      if ( pos == numbers.begin() ) {
        pos = numbers.end();
      }

      node = numbers.insert( pos, n );

      // printListCommas( numbers, out, &n );
    }
  }

  return node;
}

vector< list<long>::iterator > initial;
static void mix( list<long>& numbers, vector< list<long>::iterator > nodes )
{
  initial.clear();
  for ( auto node : nodes ) {
#ifdef DEBUG
    long n = *node;
    cout << "(" << *node << flush;
#endif
    initial.push_back( mixNode( numbers, node ) );
#ifdef DEBUG
    cout << ") " << flush;
    printListCommas( numbers, out, &n );
    fprintf( out, "\n" );
    fflush( out );
#endif
  }
}

static int getZeroIdx( const list<long>& numbers )
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

int main( int argc, char *argv[] )
{
#ifdef DEBUG
  out = fopen( "out", "w" );
#endif

  if ( argc == 2 ) {
    mixCount = atoi( argv[ 1 ] );
    if ( mixCount == -1 ) {
      mixCount = 1;
      key = 1;
    }
  }

  list<long> numbers;

  long n;
  while ( cin >> n ) {
    numbers.push_back( KEY * n );
  }

  // Check it
  FILE *parse = fopen( "PARSE", "w" );
  printList( numbers, parse );
  fclose( parse );

  // Check it reversed
  FILE *rev = fopen( "REVERSE", "w" );
  printListReverse( numbers, rev );
  fclose( rev );

  for ( auto node = numbers.begin(); node != numbers.end(); node++ )
    initial.push_back( node );

#ifdef DEBUG
  fprintf( out, "Initial arrangement:\n" );
  printListCommas( numbers, out, NULL );
  fprintf( out, "\n" );
  fflush( out );
#endif

  cout << "Mixing ... " << flush;
  // Mix them
  forn( mixCount ) {
    cout << i_ + 1 << " " << flush;
    mix( numbers, initial );
#ifdef DEBUG
  fprintf( out, "After %ld rounds of mixing:\n", i_ + 1 );
  printListCommas( numbers, out, NULL );
  fprintf( out, "\n" );
  fflush( out );
#endif
  }
  cout << "Done" << endl;

#ifdef DEBUG
  fclose( out );
#endif

  // printListCommas( numbers, stdout, NULL );
  
  int zeroIdx = getZeroIdx( numbers );
  REPORT( zeroIdx );

  // Dump it all in a vector
  vector<long> numbersVec;
  for ( long n : numbers )
    numbersVec.push_back( n );

  assert( numbersVec.size() == numbers.size() );
  int size = numbers.size();

  REPORT( numbersVec[ zeroIdx ] );

  long n1, n2, n3;
  REPORT( ( n1 = numbersVec[ ( zeroIdx + 1000 ) % size ] ) );
  REPORT( ( n2 = numbersVec[ ( zeroIdx + 2000 ) % size ] ) );
  REPORT( ( n3 = numbersVec[ ( zeroIdx + 3000 ) % size ] ) );

  long total = n1 + n2 + n3;
  REPORT( total );

  if ( KEY == 1 && mixCount == 1 )
    assert( total == 3 || total == 7225 );
  else
    switch( mixCount ) {
      case 1:
        assert( total == 2434767459 + 1623178306 + 811589153 );
        break;
      case 10:
        assert( total == 1623178306 || total == 548634267428 );
    }
}
