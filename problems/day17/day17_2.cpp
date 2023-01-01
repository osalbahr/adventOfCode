// #define VIZ

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

pl operator-(const pl& a, const pl& b) {
    return a + -1 * b;
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
// typedef map<pl,char/*,pl_hash*/> grid_t;
typedef unordered_map<pl,char,pl_hash> grid_t;

// Yes, this is the grid
// x = [0, 6]
// y = [0, inf)
grid_t grid;

typedef struct {
  int shapeIdx;
  int jetIdx;
  set<pl> environment;
} CycleDetector;

// Adapted from
// https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
bool operator<(const CycleDetector& x, const CycleDetector& y) {
    return tie(x.shapeIdx, x.jetIdx, x.environment) < tie(y.shapeIdx, y.jetIdx, y.environment);
}


// Map cycle detectors to a vector of <insertedSoFar,height>
map<CycleDetector,vector<pl>> allCycleInfo;

long height = 0;

string jetPattern;
int idx = 0;

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

static set<pl> dropletsFall( const set<pl>& waterfall )
{
  set<pl> newWaterfall;
  for ( pl droplet : waterfall ) {
    pl fall = droplet;
    fall.y--;
    if ( grid.count( fall ) == 0 )
      newWaterfall.insert( fall );
  }
  return newWaterfall;
}

// Use a "waterfall" model to find minY
static long waterfallY()
{
  set<pl> waterfall;

  // Initial droplets (hopefully compiler unrolls it)
  for ( long x = 0; x < 7; x++ )
    waterfall.insert( { x, height } );
  
  long minY = height;
  while ( minY > 0 ) {
    set<pl> tempWaterfall = waterfall;
    // First extend the layer
    for ( pl droplet : waterfall ) {
      // Try to go left
      pl pLeft = droplet + leftDiff;
      if ( droplet.x > 0 && grid.count( leftDiff ) == 0 )
        tempWaterfall.insert( pLeft );
      // Try to go right
      pl pRight = droplet + rightDiff;
      if ( droplet.x < 6 && grid.count( leftDiff ) == 0 )
        tempWaterfall.insert( pRight );
    }

    waterfall = dropletsFall( tempWaterfall );
    if ( waterfall.empty() )
      break;
    minY--;
  }

  return minY;
}

// Keep only important points
static void chopGrid() {
  grid_t newGrid;
  long maxY = height - 1;
  long minY = waterfallY() - 1; // Keep one more for vizuals
  if ( minY < -1 ) {
    REPORT( minY );
    exit( 1 );
  }
  // But don't go lower than 0
  minY = max( minY, (long)0 );
  for ( long y = maxY; y >= minY; y-- ) {
    for ( long x = 0; x < 7; x++ )
      if ( grid.count( { x, y } ) > 0 )
        newGrid[ { x, y } ] = '#';
  }
  grid = newGrid;
}

// Simply capture the environment before
// placing the shape
static set<pl> getEnvironment()
{
  set<pl> environment;
  long y = waterfallY();
  if ( y == 0 )
    return environment; // yes, empty

  // Normalize the points
  pl normalizer = { 0, -y };
  chopGrid();
  for ( const auto& [ p, ch ] : grid ) {
    // this was there only for visuals
    if ( p.y < y ) continue;

    environment.insert( p + normalizer );
  }

  return environment;
}

/**
 * @brief Returns a vector of cycle heights
 * 
 * @param shapeIdx 
 * @param insertedSoFar if -1, place normally
 * @return vector<pl> of <insertedSoFar,height>
 */
static vector<pl> placeShape( int shapeIdx, long insertedSoFar )
{
  // Insert previous info
  CycleDetector detector = {
    .shapeIdx = shapeIdx,
    .jetIdx = idx,
    .environment = getEnvironment()
  };
  pl cycleInfo = { insertedSoFar, height };
  auto it = allCycleInfo.find( detector );
  if ( it != allCycleInfo.end() && it->second.size() < 2 )
    allCycleInfo[ detector ].push_back( cycleInfo );

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

  return allCycleInfo[ detector ];
}

static void printGrid()
{
  if ( grid.size() == 0 ) {
    cout << "Empty grid" << endl;
    return;
  }

#ifdef VIZ
  // Get the height
  long maxY = height - 1;

  // Print one more, but not lower than 0
  int minY = max( waterfallY() - 2, (long)0 );
  int minX = 0, maxX = 6;
  // Print numbers
  // REPORT( height );
  cout << "    ";
  for ( int i = minX; i <= maxX; i++ ) {
    cout << i;
  }
  cout << endl;
  // bool setMinY = false;
  for ( int j = maxY; j >= minY; j-- ) {
    // bool emptyLine = true;
    printf( "%3d ", j );
    for ( int i = minX; i <= maxX; i++ ) {
      auto it = grid.find( { i, j } );
      if ( it == grid.end() ) {
        cout << '.';
      } else {
        // emptyLine = false;
        char ch = it->second;
        // if ( ch == '#' )
        //   ch = '.';
        cout << ch;
      }
    }
    cout << endl;
    // if ( emptyLine && !setMinY ) {
    //   minY = j;
    //   setMinY = true;
    // }
  }
#endif

  REPORT( height );
}

int main( int argc, char *argv[] )
{
  long x = 1000000000000;
  long n = argc == 2
  ? atol( argv[ 1 ] )
  : x;

  cin >> jetPattern;

  

  int reportFrequency = 500000;
  REPORT( reportFrequency );
  forn( n ) {
    if ( i % reportFrequency == 0 ) {
      REPORT( i );
      REPORT( ( grid.size() * ENTRY_SIZE * 2 ) / GB );
      printGrid();
      if ( i > 0 ) exit( 1 );
    }

    // REPORT( i );
    // REPORTN( i % 5 ), REPORT( idx );

    auto shapeCycleInfo = placeShape( i % 5, i );
    // chopGrid();
    // printGrid();
    if ( shapeCycleInfo.size() > 1 ) {
      pl differ = shapeCycleInfo[ 1 ] - shapeCycleInfo[ 0 ];
      // REPORTP( differ );
      if ( i + differ.first + 1 < n ) {
        i += differ.first + 1;
        i--;
        height += differ.second;
        grid_t newGrid;
        pl normalizer = { 0, differ.second };
        for ( auto& [ p, ch ] : grid ) {
          pl newP = p + normalizer;
          newGrid[ newP ] = ch;
        }
        grid = newGrid;
      }
    }

    // Assuming a load factor of 0.5
    // Keep the table < 2 GB (I only have 8 GB)
    if ( grid.size() * (ENTRY_SIZE * 2) >= 2 * GB ) {
      cout << "Chopping ... " << flush;
      chopGrid();
      cout << "Done chopping" << endl;
      printGrid();
    }
  }

  cout << "DONE placing " << n << endl;
  printGrid();
}
