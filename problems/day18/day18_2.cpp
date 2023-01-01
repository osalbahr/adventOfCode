#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>
#include <cmath>

using namespace std;

#define REPORT( X ) cout << #X << " = " << ( X ) << endl
// Like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "

typedef struct {
  int x;
  int y;
  int z;
} Cube;

// Adapted from
// https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
bool operator<(const Cube& a, const Cube& b) {
    return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z);
}

// A side is a midpoint of two cubes
typedef struct {
  double x;
  double y;
  double z;
} Side;

// Adapted from
// https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
bool operator<(const Side& a, const Side& b) {
    return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z);
}

set<Cube> cubes;
set<Side> sides;

static void printCubes( FILE *fp )
{
  for ( Cube cube : cubes )
    fprintf( fp, "%d,%d,%d\n", cube.x, cube.y, cube.z );
}

static void removeSidesBetweenCubes()
{
  for ( auto it = sides.begin(); it != sides.end(); ) {
    Side side = *it;
    assert( side.x > 0 );
    assert( side.y > 0 );
    assert( side.z > 0 );

    Cube a = { (int)side.x, (int)side.y, (int)side.z };
    Cube b = a;
    if ( side.x != (int)side.x ) {
      b.x++;
    } else if ( side.y != (int)side.y ) {
      b.y++;
    } else if ( side.z != (int)side.z ) {
      b.z++;
    } else {
      cerr << "Invalid side" << endl;
      REPORTN( side.x ), REPORTN( side.y ), REPORT( side.z );
      exit( 1 );
    }

    int countA = cubes.count( a );
    int countB = cubes.count( b );

    // Can't both be zero
    if ( countA == 0 && countB == 0 ) {
      cerr << "Invalid side" << endl;
      REPORTN( side.x ), REPORTN( side.y ), REPORT( side.z );
      REPORTN( countA ), REPORT( countB );
      exit( 1 );
    }

    // Count only non-squeezed sides
    if ( countA > 0 && countB > 0 ) {
      sides.erase( it++ );
    } else {
      it++;
    }
  }
}

static int getSurfaceArea()
{
  for ( Cube cube : cubes ) {
    double diffs[] = { -0.5, 0.5 };
    for ( double diff : diffs ) {
      sides.insert( { (double)cube.x + diff, (double)cube.y, (double)cube.z } );
      sides.insert( { (double)cube.x, (double)cube.y + diff, (double)cube.z } );
      sides.insert( { (double)cube.x, (double)cube.y, (double)cube.z + diff } );
    }
  }
  removeSidesBetweenCubes();
  return sides.size();
}

int main()
{
  string line;
  while( getline( cin, line ) ) {
    Cube cube;
    char delim;
    stringstream ss( line );
    ss >> cube.x >> delim >> cube.y >> delim >> cube.z;

    // Avoid 0
    cube.x++, cube.y++, cube.z++;
    cubes.insert( cube );
  }

  fclose( fopen( "PARSE", "w" ) );
  FILE *out = fopen( "PARSE", "w" );
  printCubes( out );
  fclose( out );

  cout << getSurfaceArea() << endl;
  REPORT( sides.size() / cubes.size() );
}
