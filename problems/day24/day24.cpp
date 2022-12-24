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

static bool detectCycle( set<pi>& positions, pi pos ) {
  return ! positions.insert( pos ).second;
}

static void printGrid( FILE *fp, const sprites_t& sprites, pi person )
{
  if ( fp == stdout )
    return;

  // Check sprite size
  int spriteSize = 0;
  for ( auto it : sprites ) {
    spriteSize++;
  }
  REPORT( sprites.size() );
  REPORT( spriteSize );

  // First line
  fprintf( fp, "#%c", person == start ? 'E' : '.' );
  for ( int i = 0; i < cols - 1; i++ )
    fprintf( fp, "#" );
  fprintf( fp, "#\n" );

  // Body
  for ( int i = 0; i < rows; i++ ) {
    fprintf( fp, "#" );

    for ( int col = 1; col <= cols; col++ )
      fprintf( fp, "%c", person == mp( i + 1, col ) ? 'E' : getCell( sprites, { i + 1, col } ) );

    fprintf( fp, "#\n" );
  }

  // Last line
  fprintf( fp, "#" );
  for ( int i = 0; i < cols - 1; i++ )
    fprintf( fp, "#" );
  fprintf( fp, "%c#\n", person == mp( rows + 1, cols ) ? 'E' : '.' );
}

static bool collision( const sprites_t& sprites, pi p, int minutes )
{
  // Safe haven
  if ( p == start || p == dest )
    return false;
  
  // Check all 4 candidates

  // up, looking v
  pi up = p + minutes * dirs[ upIdx ];
  up.row %= rows;
  if ( up.row <= 0 )
    up.row += rows;
  sprites_t::const_iterator it;
  if ( ( it = sprites.find( up ) ) != sprites.end() && it->second[ 0 ] == dirs[ downIdx ] )
    return true;


  // down, looking ^
  pi down = p + minutes * dirs[ downIdx ];
  down.row = ( down.row - 1 ) % rows + 1;
  if ( ( it = sprites.find( down ) ) != sprites.end() && it->second[ 0 ] == dirs[ upIdx ] )
    return true;
  
  // left, looking >
  pi left = p + minutes * dirs[ leftIdx ];
  left.col %= cols;
  if ( left.col <= 0 )
    left.col += cols;
  if ( ( it = sprites.find( left ) ) != sprites.end() && it->second[ 0 ] == dirs[ rightIdx ] )
    return true;
  
  // right, looking <
  pi right = p + minutes * dirs[ rightIdx ];
  right.col = ( right.col - 1 ) % cols + 1;
  if ( ( it = sprites.find( right ) ) != sprites.end() && it->second[ 0 ] == dirs[ leftIdx ] )
    return true;

  return false;
}

sprites_t initial;
int maxMinutes;
static bool reachDest( pi person, int minutes )
{
  // Shadowing
  pi start = person;

  set<pi> positions;
  queue<pi> q;
  q.push( person );
  for (;;) {
    // sprites = updateSprites( sprites );
    positions.clear();

    // printGrid( stdout, sprites, person );
    minutes++;

    if ( minutes == maxMinutes ) {
      REPORT( maxMinutes );
      return false;
    }

    // Let person explore options using DFS
    size_t size = q.size();
    if ( minutes % 100 == 0 ) {
      REPORT( minutes );
      REPORT( size );
    }
    switch( size ) {
      case 0:
        cerr << "size is 0" << endl;
        exit( 1 );
      default:
        if ( (signed)size > rows * cols + 1 ) {
          cerr << "size too big" << endl;
          REPORT( size );
          exit( 1 );
        }
    }

    for ( size_t i = 0; i < size; i++ ) {
      person = q.front();
      // printGrid( stdout, sprites, person );
      q.pop();

      for ( int i = 0; i < 5; i++ ) {
        pi dir = dirs[ i ];
        pi pos = person + dir;

        // printf( "(%d,%d) + (%d,%d) = (%d,%d)\n",
        //         person.row, person.col,
        //         dir.row,    dir.col,
        //         pos.row,    pos.col );

        if ( pos == dest ) {
          REPORT( minutes );
          return true;
        }
        
        // Don't hit a sprite or the wall
        if ( pos != start
            && ( collision( initial, pos, minutes )
            || pos.row <= 0 || pos.row > rows
            || pos.col <= 0 || pos.col > cols ) )
          continue;

        // Next layer
        if ( !detectCycle( positions, pos ) ) {
          q.push( pos );
        }
      }
    }
  }
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
  dest = { rows + 1, line.size() - 2 };
  REPORTP( dest );

  FILE *parse = fopen( "PARSE", "w" );
  printGrid( parse, initial, { -1, -1 } );
  fclose( parse );

  int minutes = 0;
  if ( !reachDest( person, minutes ) ) {
    cerr << "Never reached dest" << endl;
    exit( 1 );
  }
}
