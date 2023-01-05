#define DEBUG

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>
#include <climits>

#include <ctype.h>

using namespace std;

#ifdef DEBUG
#define REPORT( X ) cout << #X " = " << X << endl
#define REPORTP( P ) printf( "%s = (%d,%d)\n", #P, P.row+1, P.col+1 )

#define getDirStr( DIR )\
            DIR == 0 ? "Right"  :\
            DIR == 1 ? "Down"   :\
            DIR == 2 ? "Left"   :\
            DIR == 3 ? "Up"     :\
            "ERROR"

#define REPORTDIR( DIR ) printf( "%s = %s\n", #DIR, getDirStr( DIR ) )

#define REPORTWALL( NEXT ) cout << "Wall at ", REPORTP( NEXT )
#else
#define REPORT( X )
#define REPORTP( P )

#define getDirStr( DIR )

#define REPORTDIR( DIR )

#define REPORTWALL( NEXT )
#endif

typedef pair<int,int> pi;
#define row first
#define col second


vector<string> grid;
string path;
enum DIR { rightDir=0, downDir=1, leftDir=2, upDir=3 };
int dir = rightDir;
int pathIdx = 0;

static pi getStart()
{
  pi start = {0,0};
  while ( grid[ start.row ][ start.col ] == ' ' )
    start.col++; // go right

  return start;
}

static bool validCoords( pi p )
{
  // Invalid row
  if ( p.row < 0 || p.row >= grid.size() ) {
    return false;
  }
  // Invalid col
  string row = grid[ p.row ];
  if ( p.col < 0 || p.col >= row.size() ) {
    return false;
  }
  return true;
}

static pi wrap( pi p, int dir )
{
  switch( dir ) {
    // >
    case rightDir:
      if ( !validCoords( p ) )
        p.col = 0;
      while ( grid[ p.row ][ p.col ] == ' ' ) // skip empty
        p.col++;
      break;
    // <
    case leftDir:
      if ( !validCoords( p )
          || grid[ p.row ][ p.col ] == ' ' )
        p.col = grid[ p.row ].size() - 1; // skip directly
      break;
    // v
    case downDir:
      if ( !validCoords( p )
          || grid[ p.row ][ p.col ] == ' ' ) // empty
        p.row = 0;
      while ( !validCoords( p )
            || grid[ p.row ][ p.col ] == ' ' )  // skip empty
        p.row++;
      break;
    // ^
    case upDir:
      if ( !validCoords( p )
          || grid[ p.row ][ p.col ] == ' ' )  // empty
        p.row = grid.size() - 1;

      while ( !validCoords( p )
              || grid[ p.row ][ p.col ] == ' ' )  // skip empty
        p.row--;
      break;
    // For debugging only
    default:
      cerr << "Unsupported wrap ";
      REPORTDIR( dir );
      exit( 1 );
  }
  return p;
}

static pi moveDigit( pi p, int length )
{
  pi next = p;
  for ( int i = 0;; i++ ) {
    // Hit a wall
    if ( grid[ next.row ][ next.col ] == '#' ) {
      REPORTWALL( next );
      break;
    }
    // Update
    p = next;
    // Done
    if ( i >= length )
      break;

    switch( dir ) {
      case rightDir:
        next.col++;
        break;
      case downDir:
        next.row++;
        break;
      case leftDir:
        next.col--;
        break;
      case upDir:
        next.row--;
        break;
      default:
        cerr << "invalid dir: " << dir << endl;
        exit( 1 );
    }

    pi wrapped = wrap( next, dir );
    if ( next != wrapped ) {
      REPORTP( next );
      REPORTP( wrapped );
      next = wrapped;
#ifdef DEBUG
      int remaining = length - i - 1;
      REPORT( remaining );
#endif
    }
  }
  return p;
}

static pi movePath( pi p )
{
  char nextCh = path[ pathIdx ];
  int n;
  if ( isdigit( nextCh ) ) {
    int length;
    sscanf( path.c_str() + pathIdx, "%d%n", &length, &n );
#ifdef DEBUG
    cout << path.substr( pathIdx, n ) << ", ";
    REPORTDIR( dir );
#endif
    p = moveDigit( p, length );
  } else {
    n = 1;
#ifdef DEBUG
    cout << path.substr( pathIdx, n ) << endl;
#endif
    if ( nextCh == 'R' ) {
      dir = ( dir + 1 ) % 4;
    } else if ( nextCh == 'L' ) {
      if ( --dir < 0 )
        dir += 4;
    } else {
      cerr << "Unsupported ";
      REPORT( nextCh );
      exit( 1 );
    }
  }

  pathIdx += n;
  return p;
}

// How many moves
int limit = INT_MAX;

static pi getEnd( pi start )
{
  REPORTDIR( dir );
  pi end = start;
  REPORTP( end );
// #ifdef DEBUG
//   int moves = 0;
// #endif
  while ( pathIdx < (signed)path.size() ) {
    REPORT( pathIdx );
// #ifdef DEBUG
//     int oldDir = dir;
// #endif

    pi newEnd = movePath( end );
    end = newEnd;

    REPORTP( end );

// #ifdef DEBUG
//     if ( newEnd != end ) {        // move
//       REPORTP( newEnd );
//     } else if ( oldDir != dir ) { // turn
//       REPORTDIR( dir );
//     } else {
//       cout << "No move ";
//       REPORTP( end );
//     }
//     // REPORT( ++moves );
//     if ( moves == limit ) {
//       REPORT( moves );
//       exit( 1 );
//     }
// #endif
  }
  return end;
}

int main( int argc, char *argv[] )
{
  if ( argc == 2 )
    limit = atoi( argv[ 1 ] );

  string line;
  while ( getline( cin, line ) ) {
    if ( line.empty() )
      break;
    grid.push_back( line );
  }
  if ( grid.size() == 0 ) {
    cerr << "Empty grid" << endl;
    exit( 1 );
  }
  getline( cin, path );

  // pi = (x, y)
  // pi = (col-1,row-1
  pi end = getEnd( getStart() );
  REPORT( end.row + 1 );
  REPORT( end.col + 1 );
  
  printf( "End at (%d,%d), dir = %d\n", end.row+1, end.col+1, dir );
  REPORT( 1000 * ( end.row + 1 ) + 4 * ( end.col + 1 ) + dir );
  cout << 1000 * ( end.row + 1 ) + 4 * ( end.col + 1 ) + dir << endl;
}
