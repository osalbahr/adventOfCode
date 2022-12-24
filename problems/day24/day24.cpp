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
typedef pair<int,int> pi;
// Grid
#define row first
#define col second
// Raw points
// #define x first
// #define y second
#define mp( X, Y ) make_pair( X, Y )

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

typedef struct {
  pi pos;
  pi dir;
} Sprite;

// ( row, col )
// Sorted heuristically
pi dirs[]  {    { 0, 1 },  { 1, 0 }, { 0, 0 }, { 0, -1 },  { -1, 0 } };
enum Index {    rightIdx,  downIdx,  stayIdx,  leftIdx,    upIdx };

typedef map<pi,vector<Sprite*>> sprites_t;

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
    return getSpriteChar( it->second[ 0 ]->dir );
  } else {
    cerr << "Invalid cell" << endl;
    REPORT( size );
    exit( 1 );
  }
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

int main()
{
  string line;
  // First line
  getline( cin, line );
  pi person = start;
  REPORTP( person );

  cols = line.size() - 2;


  // Quickly tell if there is at least one sprite
  sprites_t initial;
  REPORT( "Parsing" );
  for ( rows = 0; getline( cin, line ) && line[ 1 ] != '#'; rows++ ) {
    for ( int col = 1; col <= cols; col++ ) {
      if ( line[ col ] != '.' ) {
        Sprite *sp = new Sprite;
        sp->pos = { rows + 1, col };
        sp->dir = getDir( line[ col ] );
        initial[ sp->pos ].push_back( sp );
      }
    }
  }
  REPORT( "Done" );

  // Last line
  dest = { rows + 1, line.size() - 1 };
  REPORTP( dest );

  FILE *parse = fopen( "PARSE", "w" );
  printGrid( parse, initial, { -1, -1 } );
  fclose( parse );
}
