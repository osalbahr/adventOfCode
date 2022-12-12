#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>

#include <cstdio>

using namespace std;

#define x first
#define y second

#define REPORT( X ) cout << #X << " = " << X << endl

vector<string> grid;

void printPath( const vector<pair<int,int>> &path )
{
  for ( auto point : path ) {
    printf( "(%d,%d)", point.x, point.y );
  }
  cout << endl;
}

void printPoint( const pair<int,int> &point )
{
  printf( "(%d,%d)", point.x, point.y );
}

int findLength( pair<int,int> start )
{
  set<pair<int,int>> visited;

  int dist = 0;
  vector<pair<int,int>> path;
  path.push_back( start );

  vector<pair<int,int>> moves = { { 0, 1 }, { 1, 0 }, { -1, 0 }, { 0, -1 } };
  
  // A queue of nodes to explore, keep alternating to know the level
  bool turn = 0;
  queue<pair<int,int>> explore;
  queue<pair<int,int>> explore2;
  explore.push( start );
  visited.insert( start );
  dist++;

  bool found = false;
  while ( !found && ( !explore.empty() || !explore2.empty() ) ) {
    if ( ( turn == 0 && explore.empty() )
      || ( turn == 1 && explore2.empty() ) ) {
      // cout << "dist = " << dist << endl;
      dist++;
      turn = !turn;
    }

    pair<int,int> current = turn == 0 ? explore.front() : explore2.front();
    turn == 0 ? explore.pop() : explore2.pop();

    int x = current.x;
    int y = current.y;
    char currentChar = grid[ x ][ y ];
    // printPoint( current );
    // cout << " is visitng:" << endl;
    for ( pair<int,int> move : moves ) {
      int newX = x + move.x;
      int newY = y + move.y;
      
      if ( newX < 0 || newX >= grid.size() )
        continue;
      if ( newY < 0 || newY >= grid[ 0 ].size() )
        continue;
      
      // cout << "newX = " << newX << ", newY = " << newY << endl;
      
      char newChar = grid[ newX ][ newY ];
      bool end = newChar == 'E';
      if ( end ) newChar = 'z';

      if ( newChar > currentChar + 1 ) {
        continue;
      }

      if ( end ) {
        // cout << "here" << endl;
        found = true;
        break;
      }

      // Try to add to the other queue
      pair<int,int> newPos = { newX, newY };
      if ( visited.insert( newPos ).second ) {
        turn == 0 ? explore2.push( newPos ) : explore.push( newPos );
      }
    }
  }

  // cout << "start = ";
  // printPoint( start );
  // cout << ", dist = " << dist << endl;
  return found ? dist : -1;
}

int main()
{
  string line;
  while ( getline( cin, line ) ) {
    grid.push_back( line );
  }

  vector<int> lengths;
  for ( int i = 0; i < grid.size(); i++ )
    for ( int j = 0; j < grid[ 0 ].size(); j++ )  
      if ( grid[ i ][ j ] == 'S' ||  grid[ i ][ j ] == 'a' ) {
        grid[ i ][ j ] = 'a';
        int length = findLength( { i, j } );
        if ( length != -1 )
          lengths.push_back( length );
      }
  
  sort( lengths.begin(), lengths.end() );

  // for ( int length : lengths )
  //   cout << length << endl;
  
  cout << lengths[ 0 ] << endl;
}
