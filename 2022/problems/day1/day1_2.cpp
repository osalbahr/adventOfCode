#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

// The top three
vector<int> top( 3 );

static void addMax( int newVal ) {
  // find min index
  int minIdx = 0;
  for ( int i = 1; i < 3; i++ )
    if ( top[ i ] < top[ minIdx ] )
      minIdx = i;
  
  // see if newVal is big enough
  if ( newVal > top[ minIdx ] )
    top[ minIdx ] = newVal;
}

int main()
{
  string line;
  int maxSoFar = 0;
  while ( getline( cin, line ) ) {
    // cout << "line = " << line << endl;
    if ( line.empty() ) {
    //   cout << maxSoFar << endl;
      addMax( maxSoFar );
      maxSoFar = 0;
      continue;
    }

    int newVal;
    sscanf( line.c_str(), "%d", &newVal );
    maxSoFar += newVal;
  }
  addMax( maxSoFar );

//   for ( int i = 0; i < 3; i++ )
//     cout << i << ": " << top[ i ] << endl;
    
  cout << top[ 0 ] + top[ 1 ] + top[ 2 ] << endl;
}
