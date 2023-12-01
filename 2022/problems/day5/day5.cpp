#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>

#include <cstdio>

#define REPORT( X ) //cout << #X << " = " << ( X ) << endl
#define REPORT3( X, Y, Z ) //REPORT( X ), REPORT( Y ), REPORT( Z )

using namespace std;

int main( int argc, char *argv[] )
{
  string line;
  int total = 0;

  // array of arrays
  int n = atoi( argv[ 1 ] );
  vector<vector<char> > aa( n );

  // get the stacks from top to bottom
  while ( getline( cin, line ) ) {
    if ( isdigit( line[ 1 ] ) )
      break;

    int idx = 1;
    for ( int i = 0; i < n; i++ ) {
      char ch = line[ idx + 4 * i ];
      if ( ch != ' ' )
        aa[ i ].push_back( ch );
    }
  }

  // reverse all arrays
  for ( auto &a : aa )
    reverse( a.begin(), a.end() );
  
  // put them in an array of stack
  vector< stack<char> > as( n );
  for ( int i = 0; i < n; i++ )
    for ( int j = 0; j < aa[ i ].size(); j++ )
      as[ i ].push( aa[ i ][ j ] );
  
  // // view the stacks
  // for ( auto s : as )
  //   cout << s.top() << s.size() << endl;


  // remove empty line
  getline( cin, line );

  // actions
  while ( getline( cin, line ) ) {
    REPORT( line );
    int moves, src, dest;
    sscanf( line.c_str(), "move %d from %d to %d", &moves, &src, &dest );
    src--;
    dest--;
    // REPORT3( moves, src, dest );
    REPORT( as[ src ].size() );
    for ( int i = 0; i < moves; i++ ) {
      REPORT( dest );
      as[ dest ].push( as[ src ].top() );
      REPORT( dest );
      as[ src ].pop();
    }
  }

  for ( auto s : as )
    cout << s.top();
  cout << endl;
}
