#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;


int main( int argc, char *argv[] )
{
  string line;
  int len = atoi( argv[ 1 ] );
  while( cin >> line ) {
    int consume = 0;
    for ( int i = 0; i < line.size() - len; i++ ) {
      set<char> s;
      for ( int j = 0; j < len; j++ )
        s.insert( line[ i + j ] );
      if ( s.size() == len ) {
        for ( int j = 0; j < len; j++ )
          cout << line[ i + j ];
        break;
      }
      consume++;
    }
    cout << consume + len << endl;
  }
}
