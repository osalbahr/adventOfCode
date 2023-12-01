#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

using namespace std;

#define REPORT( X ) cerr << #X << " = " << (X)
#define STATE() REPORT( cycle ) << ", ",\
                REPORT( strength ) << ", ",\
                REPORT( x ) << endl

int main()
{
  int strength = 0;

  int cycle = 0;
  int x = 1;

  int operand;
  string line;
  while( getline( cin, line ) ) {
    stringstream ss( line );
    string opcode;

    bool isAdd = bool( ss >> opcode >> operand );
    int cycles = isAdd ? 2 : 1;
    for ( int i = 0; i < cycles; i++ ) {
      if ( ( ++cycle - 20 ) % 40 == 0 ) {
        strength += cycle * x;
        REPORT( cycle * x ) << endl;
      }
      STATE();
    }
    
    if ( isAdd )
      x += operand;
  }

  cout << strength << endl;
}
