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

static bool adjacent( pair<int,int> a, pair<int, int> b )
{

  return abs( a.x - b.x ) <= 1 && abs( a.y - b.y ) <= 1;
}

// Let tail follow head
static pair<int,int> getNew( pair<int,int> t, pair<int,int> h )
{
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
  
  pair<int,int> h = mp( 0, 0 );
  pair<int,int> t = mp( 0, 0 );
  locations.insert( t );
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

    for ( int i = 1; i <= n; i++ ) {
      // Does tail need to move?
      pair<int,int> newH = mp( h.x + xDiff, h.y + yDiff );
      if ( !adjacent( t, newH ) ) {
        // t = h; // cannot generalize
        // Apply rules
        t = getNew( t, newH );

        locations.insert( t );
      }

      // Move head
      h = newH;

      // cout << "head: " << h.x << "," << h.y << endl;
      // cout << "tail: " << t.x << "," << t.y << endl;
      // cout << endl;
    }
  }

  cout << locations.size() << endl;
}
