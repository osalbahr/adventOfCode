#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

int main()
{
  string line;
  vector<string> grid;
    
  while ( getline( cin, line ) ) {
    grid.push_back( line );
  }

  int r = grid.size();
  int c = grid[ 0 ].size();

  int v = 0;
  for ( int i = 0; i < r; i++ ) {
    for ( int j = 0; j < c; j++ ) {
      if ( i == 0 || i == r - 1 ) {
        v++;
        continue;
      }
      if ( j == 0 || j == c - 1 ) {
        v++;
        continue;
      }

      int current = grid[ i ][ j ];

      // Visible from up
      int up = i;
      while ( --up >= 0 ) {
        if ( grid[ up ][ j ] >= current )
          break;
      }
      if ( up == -1 ) {
        v++;
        continue;
      }

      // Visible from down
      int down = i;
      while ( ++down < r ) {
        if ( grid[ down ][ j ] >= current )
          break;
      }
      if ( down == r ) {
        v++;
        continue;
      }

      // Visible from left
      int left = j;
      while ( --left >= 0 ) {
        if ( grid[ i ][ left ] >= current )
          break;
      }
      if ( left == -1 ) {
        v++;
        continue;
      }

      // Visivle from right
      int right = j;
      while ( ++right < c ) {
        if ( grid[ i ][ right ] >= current )
          break;
      }
      if ( right == c ) {
        v++;
        continue;
      }
    }
  }

  cout << v << endl;
}
