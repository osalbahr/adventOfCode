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

vector<bool> positions( 40 );

// static void printPositions()
// {
//   for ( bool pos : positions )
//     if ( pos )
//       cerr << "#"; // Lit
//     else
//       cerr << "."; // Not lit
//   cerr << endl;
// }

int main()
{

  int strength = 0;

  int cycle = 0;
  int x = 1;
  for ( int i = x - 1; i <= x + 1; i++ )
    positions[ i ] = true;

  string rendering;
  int operand;
  string line;
  while( getline( cin, line ) ) {
    // cout << line << endl;

    stringstream ss( line );
    string opcode;

    bool isAdd = bool( ss >> opcode >> operand );
    int cycles = isAdd ? 2 : 1;
    for ( int i = 0; i < cycles; i++ ) {
      // should the next rendering be lit?
      rendering += positions[ cycle % 40 ] ? "#" : ".";

      cycle++;
      if ( ( cycle - 20 ) % 40 == 0 ) {
        strength += cycle * x;
        // REPORT( cycle * x ) << endl;
      }
      if ( cycle % 40 == 0 )
        rendering += "\n";

    }
    
    if ( isAdd ) {
      x += operand;
      // REPORT( x ) << endl;
      // Move sprite
      positions.clear();
      positions.resize( 40 );
      for ( int i = x - 1; i <= x + 1; i++ ) {
        if ( i < 0 )
          continue;
        if ( i >= 40 )
          break;
        positions[ i ] = true;
      }
      // cerr << "Sprite position: ";
      // printPositions();
    }
    // STATE();
    
    // cout << rendering << endl;
  }

  cout << strength << endl;

  cout << " rendering: " << endl;
  cout << rendering << endl;
}
