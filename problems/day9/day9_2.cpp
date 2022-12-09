#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

using namespace std;

#define x first
#define y second

#define mp make_pair
#define pi pair<int,int>

static bool adjacent( pair<int,int> a, pair<int, int> b )
{

  return abs( a.x - b.x ) <= 1 && abs( a.y - b.y ) <= 1;
}

// Let tail follow head
static pair<int,int> getNew( pair<int,int> t, pair<int,int> h )
{
  // No need to change anything
  if ( adjacent( t, h ) )
    return t;

  // How far away are they?
  int xDiff = h.x - t.x;
  int yDiff = h.y - t.y;

  // What is the sign?
  int xSign = xDiff == 0 ? 0 : xDiff / abs( xDiff );
  int ySign = yDiff == 0 ? 0 : yDiff / abs( yDiff );

  // I think this follows all rules
  return mp( t.x + xSign, t.y + ySign );
}

int main()
{
  string line;

  set<pair<int,int> > locations;

  // tail at 0, head at size-1
  int size = 10;
  vector<pi> snake( size );

  locations.insert( snake[ 0 ] );
  // cout << "new: " << t.x << "," << t.y << endl;
    
  while ( getline( cin, line ) ) {
    // cout << line << endl;

    stringstream ss( line ); 
    string action;
    int n;
    ss >> action >> n;

    int xDiff = 0;
    int yDiff = 0;
    switch( action[0] ) {
      case 'R':
        xDiff = 1;
        break;
      case 'L':
        xDiff = -1;
        break;
      case 'U':
        yDiff = 1;
        break;
      case 'D':
        yDiff = -1;
        break;
    }

    // Do n moves in the desired direction
    pi *h = &snake[ size - 1 ];
    for ( int i = 1; i <= n; i++ ) {
      // Move the head
      *h = mp( h->x + xDiff, h->y + yDiff );

      // Update the snake
      for ( int j = size - 2; j >= 0; j-- ) {
        snake[ j ] = getNew( snake[ j ], snake[ j + 1 ] );
      }


      // cout << "head: " << h.x << "," << h.y << endl;
      // cout << "tail: " << t.x << "," << t.y << endl;
      // cout << endl;

      // Always insert t
      locations.insert( snake[ 0 ] );
    }
  }

  cout << locations.size() << endl;
}
