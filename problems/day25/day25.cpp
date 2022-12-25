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
#define mp make_pair

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




// Add program state (global variables) here

// static string stripSNAFU( string snafu )
// {
//   if ( snafu[ 0 ] == '0' )
//     snafu = snafu.substr( 1, snafu.size() - 1 );
//   return snafu;
// }

static string itoSNAFU( int n )
{
  REPORT( n );
  switch( n ) {
    case -2:
      return "=";
    case -1:
      return "-";
    case 0:
      return "0";
    case 1:
      return "1";
    case 2:
      return "2";
  }

  if ( n > 0 ) {
    return itoSNAFU( n / 5 ) + itoSNAFU( n % 5 );
  } else {
    cerr << "itoSNAFU: Not handled ";
    REPORT( n );
    exit( 1 );
  }
}

static int SNAFUtoi( string snafu )
{
  if ( snafu.size() == 1 ) {
    switch( snafu[ 0 ] ) {
      case '=':
        return -2;
      case '-':
        return -1;
      case '0':
        return 0;
      case '1':
        return 1;
      case '2':
        return 2;
      default:
        cerr << "Invalid digit ";
        REPORT( snafu[ 0 ] );
        exit( 1 );
    }
  }

  return 5 * SNAFUtoi( snafu.substr( 0, snafu.size() - 1 ) )
        + SNAFUtoi( snafu.substr( snafu.size() - 1, 1 ) );
}

int main()
{
  int total = 0;

  string line;
  while ( getline( cin, line ) ) {
    int n = SNAFUtoi( line );     // e = expected
    REPORT( n );
    string lina = itoSNAFU( n );  // a = actual
    if ( line != lina ) {
      REPORT( n );
      REPORT( SNAFUtoi( lina ) );
      REPORT( line );
      REPORT( lina );
      exit( 1 );
    }
    total += n;
  }
  REPORT( total );
  string snafu = itoSNAFU( total );
  int n = SNAFUtoi( snafu );
  if ( total != n ) {
    REPORT( itoSNAFU( n ) );
    REPORT( snafu );
    REPORT( n );
    exit( 1 );
  }

  REPORT( snafu );
}
