#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

#include <ctype.h>

using namespace std;

#define REPORT( X ) cout << #X " = " << X << endl
#define REPORTP( P )\
printf( "NOTE: (row,col) 1-indexed is %6s = (%d,%d)\n", #P, P.row+1, P.col+1 )

typedef pair<int,int> pi;
#define col first
#define row second


vector<string> grid;
string path;
enum DIR { rightDir=0, downDir=1, leftDir=2, upDir=3 };
int dir = rightDir;
int pathIdx = 0;

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
      if ( p.col >= currentRow.size() ) {
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
      if ( p.row >= grid.size() || grid[ p.row ][ p.col ] == ' ' ) {
        // REPORT( "Wrapping down (0)" );
        p.row = 0;
      }
      while ( grid[ p.row ][ p.col ] == ' ' ) {
        p.row++;
      }
      break;
    
    case upDir:
      if ( p.row < 0 ||  grid[ p.row ][ p.col ] == ' ' ) {
        p.row = grid.size() - 1;
      }
      while ( grid[ p.row ][ p.col ] == ' ' ) {
        p.row--;
      }
      break;

    default:
      cerr << "Unsupported wrap ";
      REPORT( dir );
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
    if ( grid[ next.row ][ next.col ] == '#' )
      break;
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

    // REPORTP( next );
    next = wrap( next, dir );
    // cout << "-> ";
    // REPORTP( next );
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
    p = moveDigit( p, length );
  } else {
    if ( nextCh == 'R' ) {
      dir = ( dir + 1 ) % 4;
    } else if ( nextCh == 'L' ) {
      dir--;
      if ( dir < 0 )
        dir += 4;
    } else {
      cerr << "Unsupported ";
      REPORT( nextCh );
      exit( 1 );
    }
    // cout << "New ";
    // REPORT( dir );
    n = 1;
  }
  // REPORT( path.substr( pathIdx ) );
  pathIdx += n;
  return p;
}

// How many moves
int limit = INT_MAX;

static pi getEnd( pi start )
{
  pi end = start;
  int moves = 0;
  while ( pathIdx < path.size() ) {
    pi newEnd = movePath( end );
    // REPORT( path.substr( pathIdx ) );
    if ( newEnd != end ) {
      // REPORTP( end );
      // REPORTP( newEnd );
      end = newEnd;
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
  getline( cin, path );

  // pi = (x, y)
  // pi = (col-1,row-1)
  pi start = getStart();
  pi end = getEnd( start );
  REPORT( end.row );
  REPORT( end.col );
  REPORT( 1000 * ( end.row + 1 ) + 4 * ( end.col + 1 ) );
}
