#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;


int main()
{
  vector<int> topThree( 3 );
  string line;
  int max = 0;
  int maxSoFar = 0;
  while ( getline( cin, line ) ) {
    // cout << line << endl;
    if ( line.empty() ) {
      // cout << maxSoFar << endl;
      if ( max < maxSoFar )
        max = maxSoFar;
      maxSoFar = 0;
      continue;
    }

    int newVal;
    sscanf( line.c_str(), "%d", &newVal );
    maxSoFar += newVal;
  }
  if ( max < maxSoFar )
    max = maxSoFar;

  cout << max << endl;
}
