#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>    // stringstream
#include <algorithm>  // sort
#include <queue>
#include <stack>

// C
#include <cstdio>
#include <cstdlib>  // atoi, malloc
#include <climits>  // INT_MIN
#include <ctype.h>  // isdigit
// #include <cstring>  // I mean, if you really wanted to

using namespace std;

// For debugging
#define REPORT( X ) cout << #X " = " << X << endl
// like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "
#define REPORTP( M ) fprintf( stdout, "%s = (%d,%d)\n", #M, M.row, M.col )

// Aliasing
#define forn( X ) \
for ( int _i = 0; _i < ( X ); _i++ )

// Point syntactic sugar
typedef int i_t;
typedef pair<i_t,i_t> pi;
// Grid
#define row first
#define col second
// Raw points
// #define x first
// #define y second
#define mp( X, Y ) make_pair( (i_t)(X), (i_t)(Y) )

// Apadted from
// https://stackoverflow.com/a/21956177/18633613
// (https://stackoverflow.com/questions/21956119/add-stdpair-with-operator)
pi operator+(const pi& a, const pi& b) {
    return make_pair(a.first + b.first, a.second + b.second);
}

// Scaling
pi operator*(const int a, const pi& p) {
    return {a * p.first, a * p.second};
}


// After template

// Dimensions of the (inner) grid
int rows, cols;

pi start = { 0, 1 };
pi dest;

// ( row, col )
// Sorted heuristically
pi dirs[]  {    { 0, 1 },  { 1, 0 }, { 0, 0 }, { 0, -1 },  { -1, 0 } };
enum Index {    rightIdx,  downIdx,  stayIdx,  leftIdx,    upIdx };

// 1-indexed points -> direction(s) of sprites
// pos -> vector<dir>
typedef map<pi,vector<pi>> sprites_t;

// direction point from ch
static pi getDir( char ch )
{
  switch ( ch ) {
    case '^':
      return dirs[ upIdx ];
    case 'v':
      return dirs[ downIdx ];
    case '<':
      return dirs[ leftIdx ];
    case '>':
      return dirs[ rightIdx ];
    default:
      cerr << "Invalid dir (getDir)" << endl;
      REPORT( ch );
      exit( 1 );
  }
}

// Essentialy, getDir^-1
static char getSpriteChar( pi dir )
{
  if ( dir == dirs[ upIdx ] )
    return '^';
  else if ( dir == dirs[ downIdx ] )
    return 'v';
  else if ( dir == dirs[ leftIdx ] )
    return '<';
  else if ( dir == dirs[ rightIdx ] )
    return '>';
  
  // else
  cerr << "Invalid dir (getDir)" << endl;
  REPORTP( dir );
  exit( 1 );
}

// P
static char getCell( const sprites_t& sprites, pi p )
{
  auto it = sprites.find( p );
  if ( it == sprites.end() )
    return '.';
  
  int size = it->second.size();
  if ( size > 1 ) {
    if ( size > 9 ) {
      return '@';
    }
    return '0' + size;
  } else if ( size == 1 ) {
    return getSpriteChar( it->second[ 0 ] );
  } else {
    cerr << "Invalid cell" << endl;
    REPORT( size );
    exit( 1 );
  }
}

static pi getNewPos( pi pos, pi dir )
{
  pi newPos = pos + dir;

  switch( newPos.row ) {
    case 0:
      newPos.row = rows;
      break;
    default:
      if ( newPos.row == rows + 1 )
        newPos.row = 1;
  }
  switch( newPos.col ) {
    case 0:
      newPos.col = cols;
      break;
    default:
      if ( newPos.col == cols + 1 )
        newPos.col = 1;
  }

  return newPos;
}

static sprites_t updateSprites( const sprites_t& oldSprites )
{
  sprites_t sprites;
  for ( auto item : oldSprites ) {
    pi pos = item.first;
    for ( pi dir : item.second ) {
      pi newPos = getNewPos( pos, dir );
      sprites[ newPos ].push_back( dir );
    }
  }
  return sprites;
}

int maxMinutes;
// Will exit
static void reachDest( const sprites_t& oldSprites, pi person, int minutes )
{
  if ( minutes == maxMinutes ) {
    exit( 1 );
  }

  sprites_t sprites = updateSprites( oldSprites );
  
  // Let person explore options recursively
  for ( int i = 0; i < 5; i++ ) {
    pi dir = dirs[ i ];
    pi pos = getNewPos( person, dir );
    if ( pos == dest )
      break;
    
    // Don't hit a sprite or the wall
    if ( sprites.count( pos ) > 0
        || pos.row == 0 || pos.row == rows + 1
        || pos.col == 0 || pos.col == cols + 1 )
      continue;
    
    // printf( "(%d,%d)->(%d,%d) dir (%d,%d)\n",
    //         person.row, person.col,
    //         pos.row,    pos.col,
    //         dir.row,    dir.col );
    // fflush( stdout );
  
    reachDest( sprites, pos, minutes + 1 );
  }

  // cerr << "Dead" << endl;
  // exit( 1 );
}

static void printGrid( FILE *fp, const sprites_t& sprites, pi person )
{
  // First line
  fprintf( fp, "#%c", person == start ? 'E' : '.' );
  for ( int i = 0; i < cols - 1; i++ )
    fprintf( fp, "#" );
  fprintf( fp, "#\n" );

  // Body
  for ( int i = 0; i < rows; i++ ) {
    fprintf( fp, "#" );

    for ( int col = 1; col <= cols; col++ )
      fprintf( fp, "%c", person == dest ? 'E' : getCell( sprites, { i + 1, col } ) );

    fprintf( fp, "#\n" );
  }

  // Last line
  fprintf( fp, "#" );
  for ( int i = 0; i < cols - 1; i++ )
    fprintf( fp, "#" );
  fprintf( fp, "%c#\n", person == mp( rows + 1, cols ) ? 'E' : '.' );
}

int main( int argc, char *argv[] )
{
  if ( argc == 2 )
    maxMinutes = atoi( argv[ 1 ] );
  else
    maxMinutes = INT_MAX;

  string line;
  // First line
  getline( cin, line );
  pi person = start;
  REPORTP( person );

  cols = line.size() - 2;


  // Quickly tell if there is at least one sprite
  sprites_t initial;
  for ( rows = 0; getline( cin, line ) && line[ 1 ] != '#'; rows++ ) {
    for ( int col = 1; col <= cols; col++ ) {
      if ( line[ col ] != '.' ) {
        pi pos = { rows + 1, col };
        pi dir = getDir( line[ col ] );
        initial[ pos ].push_back( dir );
      }
    }
  }

  // Last line
  dest = { rows + 1, line.size() - 1 };
  REPORTP( dest );

  FILE *parse = fopen( "PARSE", "w" );
  printGrid( parse, initial, { -1, -1 } );
  fclose( parse );

  int minutes = 0;
  reachDest( initial, person, minutes );

  cerr << "Never reached dest" << endl;
  exit( 1 );
}
