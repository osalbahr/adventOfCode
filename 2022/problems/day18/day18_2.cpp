#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>
#include <cmath>
#include <cassert>

using namespace std;

#define REPORT( X ) cout << #X << " = " << ( X ) << endl
// Like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "

#define REPORTC( C ) printf( "%s = (%d,%d,%d)\n", #C, (C).x, (C).y, (C).z );

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

Cube operator+( const Cube& l, const Cube& r ) {
  int x = l.x + r.x;
  int y = l.y + r.y;
  int z = l.z + r.z;
  return { x, y, z };
}

// A side is a midpoint of two cubes
typedef struct {
  float x;
  float y;
  float z;
} Side;

// Adapted from
// https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
bool operator<(const Side& a, const Side& b) {
    return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z);
}

set<Cube> cubes;
set<Side> sides;

int minX, maxX;
int minY, maxY;
int minZ, maxZ;
set<Cube> lavaCubes;

static void printCubes( FILE *fp )
{
  for ( Cube cube : cubes )
    fprintf( fp, "%d,%d,%d\n", cube.x - 1, cube.y - 1, cube.z - 1 );
}

// static void printSides()
// {
//   for ( Side s : sides ) {
//     printf( "(%.1f,%.1f,%.1f)\n", s.x - 1, s.y - 1, s.z - 1 );
//   }
// }

static Cube getOppositeCube( Side side ) {
  float diffs[] = { -0.5, 0.5 };
  bool isX = side.x != (int)side.x;
  bool isY = side.y != (int)side.y;
  bool isZ = side.z != (int)side.z;
  for ( float diff : diffs ) {
    Cube cube = { int(side.x + isX * diff),
                  int(side.y + isY * diff),
                  int(side.z + isZ * diff) };
    
    if ( cubes.count( cube ) > 0 ) {
      return {  int(side.x + isX * -diff),
                int(side.y + isY * -diff),
                int(side.z + isZ * -diff) };
    }
  }

  cerr << "Not found" << endl;
  exit( 1 );
}

static bool isLavaSide( Side side ) {
  // Look around based on orientation
  Cube oppositeCube = getOppositeCube( side );

  // REPORT( lavaCubes.count( oppositeCube ) > 0 );
  return lavaCubes.count( oppositeCube ) > 0;
}

static void dfsLava( Cube lava )
{
  // Off limits
  if (   lava.x > maxX + 1 || lava.x < minX - 1
      || lava.y > maxY + 1 || lava.y < minY - 1
      || lava.z > maxZ + 1 || lava.z < minZ - 1 )
    return;
  
  // Can't be both lava and non-lava
  if ( cubes.count( lava ) > 0 )
    return;

  // Already inserted
  if ( ! lavaCubes.insert( lava ).second )
    return;
  
  // Start dfs
  Cube differs[] = {
    { -1, 0, 0 }, { 1, 0, 0 },
    { 0, -1, 0 }, { 0, 1, 0 },
    { 0, 0, -1 }, { 0, 0, 1 }
  };
  for ( Cube differ : differs ) {
    dfsLava( lava + differ );
  }
}

// Mark the corner as a lava cube
// DFS from there
static void populateLavaCubes()
{
  // Mark the corner as a lava cube
  Cube cube = *cubes.begin();
  minX = maxX = cube.x;
  minY = maxY = cube.y;
  minZ = maxZ = cube.z;
  for ( Cube cube : cubes ) {
    minX = min( minX, cube.x );
    maxX = max( maxX, cube.x );

    minY = min( minY, cube.y );
    maxY = max( maxY, cube.y );

    minZ = min( minZ, cube.z );
    maxZ = max( maxZ, cube.z );
  }

  Cube lava = { minX - 1, minY - 1, minZ - 1 };
  dfsLava( lava );
}

// Keep only valid sides
static void filterSides()
{
  // cout << "populateLavaCubes ... " << flush;
  populateLavaCubes();
  // cout << "Done" << endl;

  assert( lavaCubes.size() );

  // Filter out sides not exposed to lava
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

    // Not exposed to air
    if ( countA > 0 && countB > 0 ) {
      sides.erase( it++ );
      continue;
    }
    
    // Not exposed to lava
    if ( !isLavaSide( side ) ) {
      sides.erase( it++ );
      continue;
    }

    // Keep it
    it++;
  }

  // // See the rest
  // printSides();
}

static int getSurfaceArea()
{
  for ( Cube cube : cubes ) {
    float diffs[] = { -0.5, 0.5 };
    for ( float diff : diffs ) {
      sides.insert( { (float)cube.x + diff, (float)cube.y, (float)cube.z } );
      sides.insert( { (float)cube.x, (float)cube.y + diff, (float)cube.z } );
      sides.insert( { (float)cube.x, (float)cube.y, (float)cube.z + diff } );
    }
  }
  filterSides();
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
}
