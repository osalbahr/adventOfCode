#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

using namespace std;

#define x first
#define y second

#define REPORT( X ) cout << #X << " = " << ( X ) << endl
#define REPORTP( P ) printf( "%s = (%d,%d)\n", #P, P.x, P.y )

// Map rock to '#'
// Map snow to 'o'
map<pair<int,int>,char> grid;

// Do not let snow fall too far down!
int lastY = INT_MIN;

void processPoints( pair<int,int> p1, pair<int,int> p2 )
{
  // REPORT( p1.x );
  // REPORT( p1.y );
  // REPORT( p2.x );
  // REPORT( p2.y );
  int x1 = p1.x, y1 = p1.y;
  int x2 = p2.x, y2 = p2.y;
  if ( x1 == x2 ) {
    int i = x1;
    // REPORT( i );
    for ( int j = min( y1, y2); j <= max( y1, y2 ); j++ ) {
      // REPORT( j );
      grid[ { i, j } ] = '#';
    }
  } else {
    int j = y1;
    // REPORT( j );
    for ( int i = min( x1, x2); i <= max( x1, x2 ); i++ ) {
      // REPORT( i );
      grid[ { i, j } ] = '#';
    }
  }
}

void parseLine( string line )
{
  stringstream ss( line );

  char comma;
  string arrow;
  int x1, y1, x2, y2;
  ss >> x1 >> comma >> y1 >> arrow >> x2 >> comma >> y2;
  do {
    processPoints( { x1, y1 }, { x2, y2 } );
    x1 = x2;
    y1 = y2;
  } while ( ss >> arrow >> x2 >> comma >> y2 );
}

bool snowing()
{
  pair<int,int> ball = { 500, 0 };

  // Try to insert
  for (;;) {
    // Out of grid
    if ( ball.y > lastY )
      return false;

    // REPORTP( ball );
    pair<int,int> down = { ball.x, ball.y + 1 };
    if ( grid.count( down ) == 0 ) {
      ball = down;
      continue;
    }

    pair<int,int> downLeft = { down.x - 1, down.y };
    if ( grid.count( downLeft ) == 0 ) {
      ball = downLeft;
      continue;
    }

    pair<int,int> downRight = { down.x + 1, down.y };
    if ( grid.count( downRight ) == 0 ) {
      ball = downRight;
      continue;
    }
    
    // Rest
    grid[ ball ] = 'o';
    return true;
  }
}

int main()
{
  string line;
    
  while ( getline( cin, line ) ) {
    // cout << line << endl;
    parseLine( line );
  }

  // Do not let snow fall too far down!
  for ( auto keyVal : grid ) {
    int y = keyVal.first.y;
    if ( y > lastY )
      lastY = y;
  }
  REPORT( lastY );

  int total = 0;
  while( snowing() )
    total += 1;
  
  cout << total << endl;
}
