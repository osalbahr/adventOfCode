#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>

#include <cstdio>

using namespace std;

#define x first
#define y second

#define REPORT( X ) cout << #X << " = " << X << endl

vector<string> grid;
set<vector<pair<int,int>>> paths;

void printPath( const vector<pair<int,int>> &path )
{
  for ( auto point : path ) {
    printf( "(%d,%d)", point.x, point.y );
  }
  cout << endl;
}

void findPaths( vector<pair<int,int>> path )
{
  // printPath( path );
  // cout << "1" << endl;
  // cout << "2" << endl;
  // Try all directions
  // cout << "c" << endl;
  pair<int,int> start = path[ path.size() - 1 ];
  char currentChar =  path.size() == 1 ?
                      'a' :
                      grid[ start.first ][ start.second ];

  // REPORT( currentChar );
  // cout << "c2" << endl;
  for ( int i = -1; i <= 1; i++ )
    for ( int j = -1; j <= 1; j++ ) {
      if ( i != 0 && j != 0 )
        continue;
      
      // REPORT( i );
      // REPORT( j );

      // cout << "ij" << endl;
      // cout << i << endl;
      // cout << j << endl;

      int newX = start.first + i;
      int newY = start.second + j;
      // cout << "what" << endl;
      if ( newX < 0 || newX >= grid.size() )
        continue;
      if ( newY < 0 || newY >= grid[ 0 ].size() )
        continue;

      // cout << "yes" << endl;

      if ( path.size() > 1 ) {
        // cout << "3" << endl;
        pair<int,int> last = path[ path.size() - 1 ];
        // cout << "4" << endl;
        if ( newX == last.first && newY == last.second )
          continue;
      }
      
      // cout << "new" << endl;
      char newChar = grid[ newX ][ newY ];
      // bool end = false;
      if ( newChar == 'E' ) {
        newChar = 'z';
      }

      // REPORT( newChar );
      // if ( newChar == 'b' && newX == 4 && newY == 1 ) {
      //   exit( 1 );
      // }
      // cout << "new2" << endl;

      pair<int,int> newPos = { newX, newY };
      if ( newChar > currentChar + 1 ) {
        // cout << "newChar > currentChar" << endl;
        continue;
      }

      // cout << "push" << endl;
      // cout << newPos.x << "," << newPos.y << endl;
      // Do not go in circles
      bool found = false;
      for ( auto prev : path ) {
        if ( prev == newPos ) {
          found = true;
          break;
        }
      }
      if ( found )
        continue;

      // cout << "before:" << endl;
      // REPORT( currentChar );
      // REPORT( grid[ path[ path.size() - 1 ].x ][ path[ path.size() - 1 ].y ] );
      // printPath( path );

      // cout << "pu" << endl;
      // printPath( path );
      // cout << newPos.x << newPos.y << endl;
      path.push_back( newPos );
      // printPath( path );
      // cout << "sh" << endl;
      // bool me = path.size() == 2 && path[ 0 ] == make_pair(0, 0) && path[ 1 ] == make_pair(0, 1);

      // printPath( path );
      // REPORT( newChar );
      // cout << "after^" << endl;
      // cout << "pop" << endl;
      
      if ( paths.insert( path ).second ) {
        // printPath( path );
        // cout << "6" << endl;

        // if ( me )
        //   cout << "recurse" << endl;
        findPaths( path );
        // if ( me )
        //   cout << "po" << endl;
        // printPath( path );
        path.pop_back();
        // printPath( path );
        // if ( me )
        //   cout << "op" << endl;
        // if ( me )
        //   cout << "SOS" << endl;
      } else {
        return;
      }
      // cout << "7" << endl;
    }

  // Never reached
  // cout << "erase1" << endl;
  // paths.erase( path );
  // cout << "erase2" << endl;
}

int main()
{
  string line;
  while ( getline( cin, line ) ) {
    grid.push_back( line );
  }

  bool foundS = false;
  bool foundE = false;
  pair<int,int> S;
  pair<int,int> E;
  for ( int i = 0; i < grid.size(); i++ ) {
    if ( foundS && foundE )
      break;

    for ( int j = 0; j < grid[ 0 ].size(); j++ ) {
      // cout << grid[ i ][ j ];
      if ( foundS && foundE )
        break;
      
      if ( grid[ i ][ j ] == 'S' ) {
        S = {i, j};
        foundS = true;
      } else if ( grid[ i ][ j ] == 'E' ) {
        E = {i, j};
        foundE = true;
      }
    }
  }

  vector<pair<int,int>> path;
  path.push_back( S );
  paths.insert( path );
  // cout << "find" << endl;
  findPaths( path );
  // cout << "no find" << endl;

  vector<int> lengths;
  // cout << "auto" << endl;
  for ( auto path : paths ) {
    printPath( path );
    // cout << "auto1.5" << endl;
    pair<int,int> last = path[ path.size() - 1 ];
    if ( grid[ last.x ][ last.y ] == 'E' ) {
      lengths.push_back( path.size() );
      REPORT( path.size() );
      // if ( path.size() == 26 ) {
      //   // printPath( path );
      //   for ( auto p : path ) {
      //     printf( "(%d,%d) = %c\n", p.x, p.y, grid[ p.x ][ p.y ] );
      //   }
      //   exit( 1 );
      // }
    }
    // cout << path.size() << endl;
  }
  // cout << "auto2" << endl;
  
  sort( lengths.begin(), lengths.end() );
  cout << lengths[ 0 ] - 1 << endl;

  // for ( auto path : paths ) {
  //   printPath( path );
  // }
}
