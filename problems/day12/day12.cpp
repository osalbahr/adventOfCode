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
  pair<int,int> start = path[ path.size() - 1 ];
  char currentChar =  path.size() == 1 ?
                      'a' :
                      grid[ start.first ][ start.second ];
  
  for ( int i = -1; i <= 1; i++ )
    for ( int j = -1; j <= 1; j++ ) {
      if ( i != 0 && j != 0 )
        continue;
      
      
      

      
      
      

      int newX = start.first + i;
      int newY = start.second + j;
      
      if ( newX < 0 || newX >= grid.size() )
        continue;
      if ( newY < 0 || newY >= grid[ 0 ].size() )
        continue;

      

      if ( path.size() > 1 ) {
        
        pair<int,int> last = path[ path.size() - 1 ];
        
        if ( newX == last.first && newY == last.second )
          continue;
      }
      
      
      char newChar = grid[ newX ][ newY ];
      
      if ( newChar == 'E' ) {
        newChar = 'z';
      }

      
      
      
      
      

      pair<int,int> newPos = { newX, newY };
      if ( newChar > currentChar + 1 ) {
        
        continue;
      }

      
      
      
      bool found = false;
      for ( auto prev : path ) {
        if ( prev == newPos ) {
          found = true;
          break;
        }
      }
      if ( found )
        continue;

      
      
      
      

      
      
      
      path.push_back( newPos );
      
      
      

      
      
      
      
      
      if ( paths.insert( path ).second ) {
        if ( paths.size() % 100000 == 0 ) {
          cout << paths.size() << endl;
          cout << path.size() << endl;
          printPath( path );
        }
        
        

        
        
        findPaths( path );
        
        
        
        path.pop_back();
        
        
        
        
        
      } else {
        return;
      }
      
    }

  
  
  
  
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
  
  findPaths( path );
  

  vector<int> lengths;
  
  for ( auto path : paths ) {
    
    
    pair<int,int> last = path[ path.size() - 1 ];
    if ( grid[ last.x ][ last.y ] == 'E' ) {
      lengths.push_back( path.size() );
      
      
      
      
      
      
      
      
    }
    
  }
  
  
  sort( lengths.begin(), lengths.end() );
  cout << lengths[ 0 ] - 1 << endl;

  
  
  
}
