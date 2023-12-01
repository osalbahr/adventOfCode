#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

#define ALL( X ) X.begin(), X.end()

using namespace std;

// basically set<char> a( str );
static set<char> findUnique( string str )
{
  set<char> result;
  for ( char ch : str )
    result.insert( ch );
  return result;
}

int main()
{
  string bag1, bag2, bag3;
  
  int total = 0;
  while ( cin >> bag1 && cin >> bag2 && cin >> bag3 ) {
    set<char> a, b, c;
    a = findUnique( bag1 );
    b = findUnique( bag2 );
    c = findUnique( bag3 );
    
    
    set<char> common12;
    set_intersection( ALL( a ), ALL( b ), inserter( common12, common12.begin() ) );
    
    set<char> sol;
    set_intersection( ALL( common12 ), ALL( c ), inserter( sol, sol.begin() ) );

    char ch = *sol.begin();
    total += ch >= 'a' ? ch - 'a' + 1 : ch - 'A' + 27;
  }

  cout << total << endl;
}
