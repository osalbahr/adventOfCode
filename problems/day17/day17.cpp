// #define VIZ

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

#include <cstdio>
#include <cmath>

using namespace std;

#define REPORT( X ) cout << #X << " = " << ( X ) << endl
#define REPORTP( P ) cout << #P << " = " << "(" << P.x << "," << P.y << ")" << endl


#define forn( X ) \
for ( int i = 0; i < ( X ); i++ )

#define x first
#define y second

typedef pair<int,int> pi;

// Apadted from
// Answer:  https://stackoverflow.com/a/21956177/18633613
// Post:    https://stackoverflow.com/questions/21956119/add-stdpair-with-operator
pi operator+(const pi& a, const pi& b) {
    return {a.x + b.x, a.y + b.y};
}

pi operator*(const int a, const pi& p) {
    return {a * p.x, a * p.y};
}

// Store offsets from the left (the bottom if there is a tie)

// ####
pi horizontal[] = { {0,0}, {1,0}, {2,0}, {3,0} };
int horizontalSize = sizeof( horizontal ) / sizeof( horizontal[ 0 ] );

// .#.
// ###
// .#.
pi plusShape[] = { {0,0}, {1,0}, {1,1}, {2,0}, {1,-1} };
int plusShapeSize = sizeof( plusShape ) / sizeof( plusShape[ 0 ] );

// ..#
// ..#
// ###
pi reverseL[] = { {0,0}, {1,0}, {2,0}, {2,1}, {2,2} };
int reverseLSize = sizeof( reverseL ) / sizeof( reverseL[ 0 ] );

// #
// #
// #
// #
pi vertical[] = { {0,0}, {0,1}, {0,2}, {0,3} };
int verticalSize = sizeof( vertical ) / sizeof( vertical[ 0 ] );

// ##
// ##
pi box[] = { {0,0}, {0,1}, {1,1}, {1,0} };
int boxSize = sizeof( box ) / sizeof( box[ 0 ] );

pi *shapes[] = { horizontal, plusShape, reverseL, vertical, box };
int sizes[] = { horizontalSize, plusShapeSize, reverseLSize, verticalSize, boxSize };

enum { horizontalIdx, plusShapeIdx, reverseLIdx, verticalIdx, boxIdx };

// Adapted from
// https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
// Only for pairs of long
// You can of course template this struct to allow other hash functions
struct pi_hash {
    long operator () (const pi &p) const {
        auto hasher = hash<int>{};

        // Not cryptographically secure, I know
        return hasher(p.x) ^ hasher(p.y);  
    }
};


// Yes, this is the grid
// x = [0, 6]
// y = [0, inf)
unordered_map<pi,char,pi_hash> grid;

int height = 0;

string jetPattern;
int idx = 0;

static void printGrid()
{
  if ( grid.size() == 0 ) {
    cout << "Empty grid" << endl;
    return;
  }

  // Get the height
  int maxY = height - 1;

#ifdef VIZ
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

  height = maxY + 1;
  REPORT( height );
}

static char jetDirection()
{
  char dir = jetPattern[ idx ];
  if ( ++idx == jetPattern.size() ) idx = 0; // quick mod
  return dir;
}

pi leftDiff = {-1,0};
pi rightDiff = {1,0};

pi down = {0,-1};

static pi jetMove( pi initial, pi *shape, int size )
{
  char dir = jetDirection();
  pi diff = dir == '>' ? rightDiff : leftDiff;

  for ( int i = 0; i < size; i++ ) {
    pi current = initial + shape[ i ] + diff;
    if ( current.x < 0 || current.x > 6 || grid.count( current ) > 0 )
      return {0,0};
  }

  // All good
  return diff;
}

static bool downMove( pi initial, pi *shape, int size )
{
  for ( int i = 0; i < size; i++ ) {
    pi current = initial + shape[ i ] + down;
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
  pi *shape = shapes[ shapeIdx ];
  int size = sizes[ shapeIdx ];

  pi initial = { 2, height + 3 }; // one more for plusShape
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
  int shapeHeight = initial.y + 1;
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

int main( int argc, char *argv[] )
{
  int n = argc == 2
  ? atoi( argv[ 1 ] )
  : 2022;

  cin >> jetPattern;
  
  forn( n )
    placeShape( i % 5 );

  printGrid();
}
