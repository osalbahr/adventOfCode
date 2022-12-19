#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>
#include <cmath>

using namespace std;

typedef struct {
  int x;
  int y;
  int z;
} Cube;

vector<Cube> cubes;

static void printCubes( FILE *fp )
{
  for ( Cube cube : cubes )
    fprintf( fp, "%d,%d,%d\n", cube.x, cube.y, cube.z );
}

static int adjacent( Cube a, Cube b )
{
  return  ( abs( a.x - b.x )
        + abs( a.y - b.y )
        + abs( a.z - b.z )  ) == 1;
}

static int getSurfaceArea()
{
  int total = 6 * cubes.size();
  for ( int i = 0; i < cubes.size(); i++ )
    for ( int j = i + 1; j < cubes.size(); j++ )
      if ( adjacent( cubes[ i ], cubes[ j ] ) )
        total -= 2;

  return total;
}

int main()
{
  string line;
  while( getline( cin, line ) ) {
    Cube cube;
    char delim;
    stringstream ss( line );
    ss >> cube.x >> delim >> cube.y >> delim >> cube.z;
    cubes.push_back( cube );
  }

  fclose( fopen( "PARSE", "w" ) );
  FILE *out = fopen( "PARSE", "w" );
  printCubes( out );
  fclose( out );

  cout << getSurfaceArea() << endl;
}
