#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

#define ALL( X ) X.begin(), X.end()

using namespace std;

int main()
{
  string bag;
  
  int total = 0;
  while ( cin >> bag ) {
    set<char> a, b;
    for( char ch : bag.substr( 0, bag.size() / 2 ) )
      a.insert( ch );

    for ( char ch : bag.substr( bag.size() / 2, bag.size() ) )
      b.insert( ch );

    set<char> sol;
    set_intersection( ALL( a ), ALL( b ),
                      inserter( sol, sol.begin() ) );

    char ch = *sol.begin();
    total += ch >= 'a' ? ch - 'a' + 1 : ch - 'A' + 27;
  }

  cout << total << endl;
}
