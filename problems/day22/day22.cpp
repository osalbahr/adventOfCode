#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>
#include <climits>

#include <ctype.h>

using namespace std;

#define REPORT( X ) cout << #X " = " << X << endl
// #define REPORTP( P ) printf( "NOTE: (row,col) 1-indexed is %6s = (%d,%d)\n", #P, P.row+1, P.col+1 )
#define REPORTP( P ) printf( "%s = (%d,%d)\n", #P, P.row+1, P.col+1 )

#define REPORTDIR( DIR ) printf( "%s = %s\n", #DIR, getDirStr( DIR ).c_str() )

typedef pair<int,int> pi;
#define col first
#define row second


vector<string> grid;
string path;
enum DIR { rightDir=0, downDir=1, leftDir=2, upDir=3 };
int dir = rightDir;
int pathIdx = 0;

static string getDirStr( int dir )
{
  return    dir == 0 ? "Right"  :
            dir == 1 ? "Down"   :
            dir == 2 ? "Left"   :
            dir == 3 ? "Up"     :
            "ERROR";

}

static pi getStart()
{
  pi start = {0,0};
  while ( grid[ start.row ][ start.col ] == ' ' ) {
    start.col++; // go right
  }
  return start;
}

static pi wrap( pi p, int dir )
{
  string currentRow = "";
  switch( dir ) {
    case rightDir:
      currentRow = grid[ p.row ];
      // REPORT( p.col );
      // for ( int i = 0; i < p.col; i++ )
      //   cout << " ";
      // cout << "v" << endl;
      // cout << currentRow << endl;
      if ( p.col >= (signed)currentRow.size() ) {
        // REPORTP( p );
        // REPORT( "Wrapping right" );
        // cout << "-> ";
        p.col = 0;
        // REPORTP( p );
      }
      while ( grid[ p.row ][ p.col ] == ' ' ) {
        // REPORTP( p );
        // cout << "->";
        p.col++;
        // REPORTP( p );
      }
      break;
    
    case leftDir:
      currentRow = grid[ p.row ];
      if ( p.col < 0 || grid[ p.row ][ p.col ] == ' ' ) {
        p.col = currentRow.size() - 1;
      }
      break;

    case downDir:
      // REPORT( p.row );
      if ( p.row >= (signed)grid.size()
          || p.col >= (signed)grid[ p.row ].size()
          || grid[ p.row ][ p.col ] == ' ' ) {
        // REPORT( "Wrapping down (0)" );
        p.row = 0;
      }
      while ( p.col >= (signed)grid[ p.row ].size() || grid[ p.row ][ p.col ] == ' ' ) {
        p.row++;
      }
      break;
    
    case upDir:
      if ( p.row < 0
          || grid[ p.row ][ p.col ] == ' ' ) {
        p.row = grid.size() - 1;
      }
      while ( p.col >= (signed)grid[ p.row ].size() || grid[ p.row ][ p.col ] == ' ' ) {
        p.row--;
      }
      break;

    default:
      cerr << "Unsupported wrap ";
      REPORTDIR( dir );
      exit( 1 );
  }

  return p;
}

static pi moveDigit( pi p, int length )
{
  // REPORT( length );

  pi next = p;
  for ( int i = 0;; i++ ) {
    // REPORT( i );
    // Hit a wall
    if ( grid[ next.row ][ next.col ] == '#' ) {
      cout << "Wall at ";
      REPORTP( next );
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
      int remaining = length - i - 1;
      REPORT( remaining );
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
    int ret;
    if ( ( ret = sscanf( path.c_str() + pathIdx, "%d%n", &length, &n ) ) != 1 ) {
      cerr << "Not a digit" << endl;
      REPORT( ret );
      REPORT( path );
      REPORT( pathIdx );
      REPORT( nextCh );
      REPORT( isdigit( nextCh ) );
      exit( 1 );
    }
    cout << path.substr( pathIdx, n ) << ", ";
    REPORTDIR( dir );

    p = moveDigit( p, length );
  } else {
    n = 1;
    cout << path.substr( pathIdx, n ) << endl;

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
  int moves = 0;
  while ( pathIdx < (signed)path.size() ) {
    int oldDir = dir;
    pi newEnd = movePath( end );
    if ( newEnd != end ) {        // move
      end = newEnd;
      REPORTP( end );
    } else if ( oldDir != dir ) { // turn
      REPORTDIR( dir );
    } else {
      cout << "No move ";
      REPORTP( end );
    }
    // REPORT( ++moves );
    if ( moves == limit ) {
      REPORT( moves );
      exit( 1 );
    }
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
  // pi = (col-1,row-1)
  pi start = getStart();
  pi end = getEnd( start );
  REPORT( end.row );
  REPORT( end.col );
  REPORT( 1000 * ( end.row + 1 ) + 4 * ( end.col + 1 ) );
}
