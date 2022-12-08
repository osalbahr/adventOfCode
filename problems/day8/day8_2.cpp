#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

#define long int

vector<string> grid;

int rows, cols;

static long getScore( int row, int col )
{
  long upC = 0, leftC = 0, downC = 0, rightC = 0;
  int current = grid[ row ][ col ];

  int i = row;
  int j = col;

  // Visible from up
  int up = i;
  while ( --up >= 0 ) {
    upC++;
    if ( grid[ up ][ j ] >= current )
      break;
  }


  // Visible from down
  int down = i;
  while ( ++down < rows ) {
    downC++;
    if ( grid[ down ][ j ] >= current )
      break;
  }

  // Visible from left
  int left = j;
  while ( --left >= 0 ) {
    leftC++;
    if ( grid[ i ][ left ] >= current )
      break;
  }

  // Visivle from right
  int right = j;
  while ( ++right < cols ) {
    rightC++;
    if ( grid[ i ][ right ] >= current )
      break;
  }

  if ( upC == 0 ) upC = 1;
  if ( leftC == 0 ) leftC = 1;
  if ( downC == 0 ) downC = 1;
  if ( rightC == 0 ) rightC = 1;

  return upC * leftC * downC * rightC;
}

int main()
{
  string line;
    
  while ( getline( cin, line ) ) {
    grid.push_back( line );
  }

  rows = grid.size();
  cols = grid[ 0 ].size();

  vector<long> scores;
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j < cols; j++ ) {
      long score = getScore( i, j );
      scores.push_back( score );
      // cout << "i = " << i << ", j = " << j << ", score = " << score << endl;
    }
  }

  sort( scores.begin(), scores.end() );
  cout << scores[ scores.size() - 1 ] << endl;
}
