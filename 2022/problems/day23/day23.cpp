#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>
#include <climits>

using namespace std;

#define REPORT( X ) cout << #X " = " << X << endl
#define REPORTP( M ) fprintf( stdout, "%s = (%d,%d)\n", #M, M.row, M.col )

#define forn( X ) \
for ( int i = 0; i < ( X ); i++ )


typedef pair<int,int> pi;
#define row first
#define col second

// Apadted from
// https://stackoverflow.com/a/21956177/18633613
// (https://stackoverflow.com/questions/21956119/add-stdpair-with-operator)
pi operator +(const pi& a, const pi& b) {
    return make_pair(a.row + b.row, a.col + b.col);
}

// Program state
set<pi> monkeys;
pi N = {-1,0}, S = {1,0}, W = {0,-1}, E = {0,1};
pi NE = N + E, NW = N + W;
pi SE = S + E, SW = S + W;
pi dirs[] = { N, S, W, E };
int start = 0;

static void printMonkeys( FILE *fp )
{
  int minRow = INT_MAX, minCol = INT_MAX;
  int maxRow = INT_MIN, maxCol = INT_MIN;
  for ( pi m : monkeys ) {
    if ( m.row < minRow ) minRow = m.row;
    if ( m.row > maxRow ) maxRow = m.row;
    if ( m.col < minCol ) minCol = m.col;
    if ( m.col > maxCol ) maxCol = m.col;
  }

  // // Override for testing sample input (1.IN)
  // minRow = 1 - 2;
  // maxRow = 7 + 3;
  // minCol = 1 - 3;
  // maxCol = 7 + 4;

  // // Override to fix the size
  // minRow = 1;// - 2;
  // maxRow = 7;// + 3;
  // minCol = 1;// - 3;
  // maxCol = 7;// + 4;

  for ( int row = minRow; row <= maxRow; row++ ) {
    for ( int col = minCol; col <= maxCol; col++ ) {
      pi current = { row, col };
      char ch = monkeys.count( current ) == 0 ? '.' : '#';
      fprintf( fp, "%c", ch );
    }
    fprintf( fp, "\n" );
  }

  for ( pi m : monkeys ) {
    if ( m.row < minRow ) minRow = m.row;
    if ( m.row > maxRow ) maxRow = m.row;
    if ( m.col < minCol ) minCol = m.col;
    if ( m.col > maxCol ) maxCol = m.col;
  }

  int rows = maxRow - minRow + 1;
  int cols = maxCol - minCol + 1;
  // REPORT( rows );
  // REPORT( cols );
  int total = rows * cols;
  // REPORT( total );
  // REPORT( monkeys.size() );
  int empty = total - monkeys.size();
  REPORT( empty );
  cout << endl;
}

static pi getPropose( pi m )
{
  // Look around to check stay case
  bool stay = true;
  for ( int rowDiff = -1; stay && rowDiff <= 1; rowDiff++ )
    for ( int colDiff = -1; stay && colDiff <= 1; colDiff++ ) {
      if ( rowDiff == 0 && colDiff == 0 )
        continue;
      
      // Found a neighbor
      pi p = { m.row + rowDiff, m.col + colDiff };
      if ( monkeys.count( p ) > 0 )
        stay = false;
    }
  if ( stay )
    return m;
  
  for ( int i = 0; i < 4; i++ ) {
    pi dir = dirs[ ( start + i ) % 4 ];
    if ( dir == N ) {
      if ( monkeys.count( m + N ) == 0
          && monkeys.count( m + NE ) == 0 
          && monkeys.count( m + NW ) == 0 )
        return m + N;
    } else if ( dir == S ) {
      if ( monkeys.count( m + S ) == 0
          && monkeys.count( m + SE ) == 0 
          && monkeys.count( m + SW ) == 0 )
        return m + S;
    } else if ( dir == W ) {
      if ( monkeys.count( m + W ) == 0
          && monkeys.count( m + NW ) == 0 
          && monkeys.count( m + SW ) == 0 )
        return m + W;
    } else if ( dir == E ) {
      if ( monkeys.count( m + E ) == 0
          && monkeys.count( m + NE ) == 0 
          && monkeys.count( m + SE ) == 0 )
        return m + E;
    } else {
      cerr << "Invalid dir: ";
      REPORTP( dir );
      exit( 1 );
    }
  }

  // No propose?
  return m;
  
  // You shouldn't get here
  cerr << "Could not propose " << endl;
  REPORTP( m );
  printMonkeys( fopen( "ERROR", "w" ) );
  exit( 1 );
}

static void round()
{
  // Store proposes
  vector<pi> pVec;
  multiset<pi> pSet;

  // Store order of monkeys
  vector<pi> mVec;

  for ( pi m : monkeys ) {
    mVec.push_back( m );

    pi p = getPropose( m );
    pVec.push_back( p );
    pSet.insert( p );
  }

  int i = 0;
  for ( pi m : mVec ) {
    pi p = pVec[ i++ ];
    if ( m != p && pSet.count( p ) == 1 ) {
  
      if ( monkeys.count( p ) ) {
        cout << endl << "Stepping: " << endl;
        cerr << "You can't step on a monkey!" << endl;
        exit( 1 );
      }

      monkeys.erase( m );
      monkeys.insert( p );
    }
  }

  start++;
}

int main( int argc, char *argv[] )
{
  if ( argc != 2 ) {
    cout << "day22 <rounds>" << endl;
    exit( 1 );
  }
  int rounds = atoi( argv[ 1 ] );
  
  string line;
  for( int row = 1; getline( cin, line ); row++ )
    for ( int i = 0; i < line.size(); i++ )
      if ( line[ i ] == '#' )
        monkeys.insert( { row, i + 1 } );
  
  FILE *parse = fopen( "PARSE", "w" );
  printMonkeys( parse );
  fclose( parse );

  FILE *state = fopen( "STATE", "w" );
  fprintf( state, "== Initial State ==\n" );
  fprintf( stdout, "== Initial State ==\n" );
  printMonkeys( state );
  fprintf( state, "\n" );

  forn( rounds ) {
    round();
    fprintf( state, "== End of Round %d ==\n", i + 1 );
    fprintf( stdout, "== End of Round %d ==\n", i + 1 );
    printMonkeys( state );
    fprintf( state, "\n" );
    fprintf( stdout, "\n" );
  }

  fclose( state );
}
