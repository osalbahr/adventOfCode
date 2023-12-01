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
  int a1, a2, b1, b2;
  int total = 0;
  while ( scanf( "%d-%d,%d-%d", &a1, &a2, &b1, &b2 ) == 4 ) {
    cout << a1 << "-" << a2 << endl;
    cout << b1 << "-" << b2 << endl;
    if ( a1 <= b1 && a2 >= b2
        || b1 <= a1 && b2 >= a2 ) {
      total += 1;
      cout << "YES" << endl;
    } else {
      cout << "NO" << endl;
    }
  }

  cout << total << endl;
}
