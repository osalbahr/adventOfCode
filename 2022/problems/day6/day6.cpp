#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

bool unique( char a, char b, char c, char d )
{
  return a != b && a != c && a != d
      && b != c && b != d
      && c != d;
}

int main()
{
  string line;
  while( cin >> line ) {
    int consume = 0;
    for ( int i = 0; i < line.size() - 3; i++ ) {
      char a, b, c, d;
      a = line[ i ];
      b = line[ i + 1 ];
      c = line[ i + 2 ];
      d = line[ i + 3 ];
      if ( unique( a, b, c, d ) ) {
        cout << a << b << c << d << " : ";
        break;
      }
      consume++;
    }
    cout << consume + 4 << endl;
  }
}
