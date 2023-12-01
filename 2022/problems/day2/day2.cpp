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
  char opp, me;
  while ( cin >> a >> b ) {
    opp = a[ 0 ];
    me = b[ 0 ];
    
    opp -= 'A';
    me -= 'X';

    // if i am 1 ahead, I win
    int ahead = ( me - opp + 3 ) % 3;
    cout << ":" << ahead << endl;
    if ( ahead == 1 )
      total += 6;
    
    // if i am 2 ahead, I lose
    if ( ahead == 2 )
      total += 0;
    
    // Redundant check for tie
    if ( ahead == 0 )
      total += 3;


    // Char value
    total += me + 1;

    cout << total << endl;
  }

  cout << total << endl;
}
