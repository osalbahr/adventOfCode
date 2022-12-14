#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <fstream>

#include <cstdio>
#include <cmath>

using namespace std;

#define REPORT( X ) cout << #X << " = " << ( X ) << endl

#define forn( X ) \
for ( int i = 0; i < ( X ); i++ )

#define pi pair<int,int>
#define x first
#define y second

int lineOfInterest;
int leftIdx = INT_MAX;
int rightIdx = INT_MIN;

map<pi,char> grid;

// Number of places a beacon cannot exist
map<int,int> cannot;

typedef struct BeaconTag {
  pi coords;
} Beacon;

typedef struct SensorTag {
  pi coords;
  Beacon beacon;

  // Sensor at x=?, y=?: closest beacon is at x=?, y=?
  struct SensorTag operator() ( string line ) {
    struct SensorTag sensor;

    stringstream ss( line );
    string ignoreString;
    char ignoreChar;
    //Sensor at"
    forn( 2 )
      ss >> ignoreString;
    //x=
    forn( 2 )
      ss >> ignoreChar;
    // get x
    ss >> sensor.coords.x;
    //, y=
    forn( 3 )
      ss >> ignoreChar;
    // get y
    ss >> sensor.coords.y;
    //:
    forn( 1 )
      ss >> ignoreChar;
    // closest beacon is at
    forn( 4 )
      ss >> ignoreString;
    //x=
    forn( 2 )
      ss >> ignoreChar;
    // get x
    ss >> sensor.beacon.coords.x;
    //, y=
    forn( 3 )
      ss >> ignoreChar;
    // get y
    ss >> sensor.beacon.coords.y;

    return sensor;
  }
} Sensor;

vector<Sensor> sensors;

int distance( Sensor sensor )
{
  pi s = sensor.coords;
  pi b = sensor.beacon.coords;
  return abs( s.x - b.x ) + abs( s.y - b.y );
}

void mark( Sensor sensor )
{
  // Mark with #
  int dist = distance( sensor );
  // REPORT( dist );
  int x = sensor.coords.x;
  int y = sensor.coords.y;
  REPORT( dist );
  if ( abs( y - lineOfInterest ) > dist ) {
    REPORT( y - lineOfInterest );
    return;
  }
  // REPORT( y );
  // for ( int r = lineOfInterest; r <= lineOfInterest; r++ ) {
    int r = lineOfInterest;
  // REPORT( r );
  int rem = dist - abs( y - r );
  // REPORT( rem );
  if ( x - rem < leftIdx )
    leftIdx = x - dist;
  if ( x + rem > rightIdx )
    rightIdx = x + dist;

    // REPORT( r );
    // REPORT( c );
    // pair<int,int> poi = { c, r };
    // if ( grid.find( poi ) == grid.end() ) {
    //   grid[ poi ] = '#';
    //   cannot[ r ]++;
    // }
    // REPORT( "before" );
    // REPORT( it->second );
    // printf( "(%d,%d) = #\n", r, c );
    // grid[ poi ] = '#';

    // cout << "Printing grid" << endl;
    // for ( auto itemPair : grid ) {
    //   pi point = itemPair.first;
    //   REPORT( point.y );
    // }

    // REPORT( "after" );
    // REPORT( it->second );
    // cannot[ r ]++;
    // if ( r == 10 ) {
    //   REPORT( c );
    //   REPORT( cannot[ r ] );
    // }
  // } 

  // Mark S
  pair<int,int> sp = sensor.coords;
  grid[ sp ] = 'S';
  // printf( "Mark S x=%d, y=%d\n", sensor.coords.x, sensor.coords.y );

  // Mark B
  pair<int,int> bp = sensor.beacon.coords;
  if ( grid.find( bp ) == grid.end() ) {
    grid[ bp ] = 'B';
    cannot[ bp.y ]--;
  }
}

static void printSensor( Sensor s )
{
  FILE *out = fopen( "1.PARSE", "a" );
  auto sc = s.coords;
  auto bc = s.beacon.coords;
  fprintf( out, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n",
                            sc.x, sc.y,                      bc.x, bc.y );
}

int main( int argc, char *argv[] )
{
  if ( argc != 2 ) {
    cerr << "Usage: day15 <lineOfInterest>" << endl;
    exit( 1 );
  }

  lineOfInterest = atoi(argv[ 1 ]);

  fclose( fopen( "1.PARSE", "w" ) );

  string line;

  while ( getline( cin, line ) ) {
    Sensor sensor = Sensor()( line );
    sensors.push_back( sensor );
    
    printSensor( sensor );
  }

  cout << "Before:" << endl;
  int minX;
  int maxX;
  int minY;
  int maxY;
  minX = INT_MAX, maxX = INT_MIN;
  minY = INT_MAX, maxY = INT_MIN;
  for ( auto itemPair : grid ) {
    pi point = itemPair.first;
    if ( point.x < minX )
      minX = point.x;
    if ( point.x > maxX )
      maxX = point.x;
    if ( point.y < minY )
      minY = point.y;
    if ( point.y > maxY )
      maxY = point.y;
  }

  for ( int i = minX; i <= maxX; i++ ) {
    for ( int j = minY; j <= maxY; j++ ) {
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

  for ( Sensor sensor : sensors ) {
    REPORT( sensor.coords.y );
    mark( sensor );
  }
  
  minX = INT_MAX, maxX = INT_MIN;
  minY = INT_MAX, maxY = INT_MIN;
  for ( auto itemPair : grid ) {
    pi point = itemPair.first;
    // REPORT( point.x );
    // REPORT( point.y );
    if ( point.x < minX )
      minX = point.x;
    if ( point.x > maxX )
      maxX = point.x;
    if ( point.y < minY )
      minY = point.y;
    if ( point.y > maxY )
      maxY = point.y;
  }

  cout << "After:" << endl;
  // minX = -2;
  // maxX = 25;
  // minY = 0;
  // maxY = 22;

  // minX = maxX = 2;
  // minY = maxY = 18;

  // Get height of col number
  // Add 1 for sign or zero
  // int temp = minY;
  // // REPORT( minY );
  // int height = temp <= 0 ? 1 : 0;
  // temp /= 10;
  // while ( ( temp /= 10 ) != 0 )
  //   height++;
  // // REPORT( height );

  // int temp2 = maxY;
  // int height2 = temp2 <= 0 ? 1 : 0;
  // temp /= 10;
  // while ( ( temp /= 10 ) != 0 )
  //   height2++;
  
  // height = max( height, height2 );

  // // Print numbers
  // // REPORT( height );
  // for ( int div = pow( 10, height ); div != 0; div /= 10 ) {
  //   cout << "   ";
  //   // REPORT( div );
  //   for ( int x = minX; x <= maxX; x++ ) {
  //     if ( x % 5 == 0 ) {
  //       int digit = ( abs(x) / div ) % 10;
  //       if ( digit == 0 && div != 1 ) {
  //         // Look ahead
  //         if ( x < 0 && ( abs(x) / (div/10) ) % 10 != 0 )
  //           cout << "-";
  //         else
  //           cout << " ";
  //       }
  //       else
  //         cout << digit;
  //     } else {
  //       cout << " ";
  //     }
  //   }
  //   cout << endl;
  // }

  // for ( int j = minY; j <= maxY; j++ ) {
  //   printf( "%3d ", j );
  //   for ( int i = minX; i <= maxX; i++ ) {
  //     auto it = grid.find( { i, j } );
  //     if ( it == grid.end() )
  //       cout << '.';
  //     else {
  //       char ch = it->second;
  //       // if ( ch == '#' )
  //       //   ch = '.';
  //       cout << ch;
  //     }
  //   }
  //   cout << endl;
  // }

  // for ( auto pair : grid ) {
  //   REPORT( pair.first.x );
  //   REPORT( pair.first.y );
  //   REPORT( pair.second );
  // }

 REPORT( lineOfInterest );
 REPORT( cannot[ lineOfInterest ] );
 REPORT( leftIdx );
 REPORT( rightIdx );
 REPORT( rightIdx - leftIdx + 1 );
}
