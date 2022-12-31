// #define VIZ
// #define CHOP

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

#include <cstdio>
#include <cmath>

using namespace std;

// #define endl "\n"

#define REPORT( X ) cout << #X << " = " << ( X ) << endl
// Like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "

#define REPORTP( P ) cout << #P << " = " << "(" << P.x << "," << P.y << ")" << endl

#define GB ( (long)1024 * 1024 * 1024 )

#define forn( X ) \
for ( long i = 0; i < ( X ); i++ )

#define x first
#define y second

typedef pair<long,long> pl;

// Apadted from
// Answer:  https://stackoverflow.com/a/21956177/18633613
// Post:    https://stackoverflow.com/questions/21956119/add-stdpair-with-operator
pl operator+(const pl& a, const pl& b) {
    return {a.x + b.x, a.y + b.y};
}

pl operator*(const int a, const pl& p) {
    return {a * p.x, a * p.y};
}

// Store offsets from the left (the bottom if there is a tie)

// ####
pl horizontal[] = { {0,0}, {1,0}, {2,0}, {3,0} };
int horizontalSize = sizeof( horizontal ) / sizeof( horizontal[ 0 ] );

// .#.
// ###
// .#.
pl plusShape[] = { {0,0}, {1,0}, {1,1}, {2,0}, {1,-1} };
int plusShapeSize = sizeof( plusShape ) / sizeof( plusShape[ 0 ] );

// ..#
// ..#
// ###
pl reverseL[] = { {0,0}, {1,0}, {2,0}, {2,1}, {2,2} };
int reverseLSize = sizeof( reverseL ) / sizeof( reverseL[ 0 ] );

// #
// #
// #
// #
pl vertical[] = { {0,0}, {0,1}, {0,2}, {0,3} };
int verticalSize = sizeof( vertical ) / sizeof( vertical[ 0 ] );

// ##
// ##
pl box[] = { {0,0}, {0,1}, {1,1}, {1,0} };
int boxSize = sizeof( box ) / sizeof( box[ 0 ] );

pl *shapes[] = { horizontal, plusShape, reverseL, vertical, box };
int sizes[] = { horizontalSize, plusShapeSize, reverseLSize, verticalSize, boxSize };

enum shapeIdx { horizontalIdx, plusShapeIdx, reverseLIdx, verticalIdx, boxIdx };

// Adapted from
// https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
// Only for pairs of long
// You can of course template this struct to allow other hash functions
struct pl_hash {
    long operator () (const pl &p) const {
        auto hasher = hash<long>{};
        // Not cryptographically secure, I know
        return hasher(p.x) ^ hasher(p.y);  
    }
};

#define ENTRY_SIZE ( sizeof( pl ) + sizeof( char ) )

// Yes, this is the grid
// x = [0, 6]
// y = [0, inf)
unordered_map<pl,char,pl_hash> grid;

typedef struct {
  shapeIdx currentShapeIdx;
  int jetIdx;
  int height;
} CycleDetector;


// Map cycle detectors to a vector of heights
map<CycleDetector,vector<int>> cycleHeights;

long height = 0;

string jetPattern;
int idx = 0;

static void printGrid()
{
  if ( grid.size() == 0 ) {
    cout << "Empty grid" << endl;
    return;
  }

#ifdef VIZ
  // Get the height
  long maxY = height - 1;

  int minY = 0;
  int minX = 0, maxX = 6;
  // Print numbers
  // REPORT( height );
  cout << "    ";
  for ( int i = minX; i <= maxX; i++ ) {
    cout << i;
  }
  cout << endl;
  for ( int j = maxY; j >= minY; j-- ) {
    printf( "%3d ", j );
    for ( int i = minX; i <= maxX; i++ ) {
      auto it = grid.find( { i, j } );
      if ( it == grid.end() )
        cout << '.';
      else {
        char ch = it->second;
        // if ( ch == '#' )
        //   ch = '.';
        cout << ch;
      }
    }
    cout << endl;
  }
#endif

  REPORT( height );
}

static char jetDirection()
{
  char dir = jetPattern[ idx ];
  if ( ++idx == jetPattern.size() ) idx = 0; // quick mod
  return dir;
}

pl leftDiff = {-1,0};
pl rightDiff = {1,0};

pl down = {0,-1};

static pl jetMove( pl initial, pl *shape, int size )
{
  char dir = jetDirection();
  pl diff = dir == '>' ? rightDiff : leftDiff;

  for ( int i = 0; i < size; i++ ) {
    pl current = initial + shape[ i ] + diff;
    if ( current.x < 0 || current.x > 6 || grid.count( current ) > 0 )
      return {0,0};
  }

  // All good
  return diff;
}

static bool downMove( pl initial, pl *shape, int size )
{
  for ( int i = 0; i < size; i++ ) {
    pl current = initial + shape[ i ] + down;
    if ( current.y < 0 || grid.count( current ) > 0 )
      return false;
  }

  return true;
}

// // Temporarily put, print, erase a shape
// static void printGridTemp( pi initial, pi *shape, int size )
// {
//   for ( int i = 0; i < size; i++ )
//     grid[ initial + shape[ i ] ] = '@';
//   printGrid();
//   for ( int i = 0; i < size; i++ )
//     grid.erase( initial + shape[ i ] );
// }

static void placeShape( int shapeIdx )
{
  pl *shape = shapes[ shapeIdx ];
  int size = sizes[ shapeIdx ];

  pl initial = { 2, height + 3 }; // one more for plusShape
  if ( shape == plusShape )
    initial.y++;

  // Now we determined initial location, let it fall
  bool station = false;
  while ( !station ) {
    initial = initial + jetMove( initial, shape, size );
    bool canDown;
    if ( ( canDown = downMove( initial, shape, size ) ) ) {
      initial = initial + down;
    } else {
      station = true;
    }
  }

  for ( int i = 0; i < size; i++ ) {
    grid[ initial + shape[ i ] ] = '#';
  }

  // Update height
  long shapeHeight = initial.y + 1;
  switch( shapeIdx ) {
    case horizontalIdx:
      // Do nothing
      break;
    case plusShapeIdx:
      shapeHeight += 1;
      break;
    case reverseLIdx:
      shapeHeight += 2;
      break;
    case verticalIdx:
      shapeHeight += 3;
      break;
    case boxIdx:
      shapeHeight += 1;
      break;
    default:
      cerr << "Unkown" << endl;
      REPORT( shapeIdx );
      exit( 1 );
  }

  height = max( height, shapeHeight );
}

#ifdef CHOP
// Keep only the last 100
static void chopGrid() {
  unordered_map<pl,char,pl_hash> newGrid;
  int maxY = height - 1;
  for ( int y = maxY; y > maxY - 100; y-- ) {
    for ( int x = 0; x < 7; x++ )
      if ( grid.count( { x, y } ) > 0 )
        newGrid[ { x, y } ] = '#';
  }
  grid = newGrid;
}
#endif

int main( int argc, char *argv[] )
{
  long x = 1000000000000;
  long n = argc == 2
  ? atol( argv[ 1 ] )
  : x;

  cin >> jetPattern;

  

  int reportFrequency = 5000000;
  REPORT( reportFrequency );
  forn( n ) {
    if ( i % reportFrequency == 0 ) {
      REPORT( i );
      REPORT( ( grid.size() * ENTRY_SIZE * 2 ) / GB );
      printGrid();
    }

    placeShape( i % 5 );

#ifdef CHOP
    // Assuming a load factor of 0.5
    // Keep the table < 2 GB (I only have 8 GB)
    if ( grid.size() * (ENTRY_SIZE * 2) >= 2 * GB ) {
      cout << "Chopping ... " << flush;
      chopGrid();
      cout << "Done chopping" << endl;
    }
#endif
  }

  printGrid();
}
