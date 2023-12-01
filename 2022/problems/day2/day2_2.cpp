#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

void solve()
{
  
}

int main()
{
  int total = 0;
  string a, b;
  while ( cin >> a >> b ) {
    char opp = a[ 0 ];
    opp -= 'A';

    char me = b[ 0 ];
    me -= 'X';

    // Now for "me"
    // 0 = lose
    // 1 = draw
    // 2 = win

    // if i am 1 ahead, I win
    if ( me == 2 )
      me = ( opp + 1 ) % 3 + 6;
    
    // if i am 2 ahead, I lose
    else if ( me == 0 )
      me = ( opp + 2 ) % 3 + 0;
    
    // Redundant check for tie
    else // ( me == 1 )
      me = opp + 3;

    total += me + 1;
  }

  cout << total << endl;
}
