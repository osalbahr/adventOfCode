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
#include <cmath>
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

#define minabs( X, Y )\
min( abs( X ), abs( Y ) )

static int getExponent( long n )
{
  // REPORT( n );

  n = abs( n );

  // REPORT( n );
  int exp = 0;
  long total = 2;

  // Find the minimum needed number of digits
  while ( n > total ) {
    // REPORT( n );
    // REPORT( total );

    total *= 5;
    total += 2;
    exp++;
  }

  // REPORT( exp );
  // REPORT( pow( 5, exp ) );
  return exp;
}

static string mergeSNAFU( string leading, string rest, int exp )
{
  string snafu = leading;
  // Fill with zeros
  if ( rest.size() > exp ) {
    cerr << "Invalid merge" << endl;
    REPORT( leading );
    REPORT( rest );
    REPORT( exp );
    exit( 1 );
  }

  for ( int i = 0; i < exp - rest.size(); i++ ) {
    // REPORT( i );
    snafu += '0';
  }

  return snafu + rest;
}

static string ltoSNAFU( long n )
{
  // Base case: abs( n ) <= 2
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

  int exp = getExponent( n );
  long raised = pow( 5, exp );

  // REPORT( n );
  // REPORT( exp );
  // REPORT( raised );

  // If it is negative, the leading is negative
  if ( n < 0 ) {
    long m = abs( n );
    // Try to minimize, assume aggressive
    if ( abs( m - 2 * raised ) < abs( m - raised ) ) {
      long rem = n - ( -2 * raised );
      // REPORT( "=" );
      return mergeSNAFU( "=", ltoSNAFU( rem ), exp );
    } else if ( abs( m - raised ) < m ) {
      long rem = n - ( -1 * raised );
      // REPORT( "-" );
      return mergeSNAFU( "-", ltoSNAFU( rem ), exp );
    } else {
      cerr << "Unexpected (n < 0)" << endl;
      REPORT( n );
      exit( 1 );
    }
  } else {
    // Try to minimize
    if ( abs( n - 2 * raised ) < abs( n - raised ) ) {
      long rem = n - ( 2 * raised );
      // REPORT( "2" );
      return mergeSNAFU( "2", ltoSNAFU( rem ), exp );
    } else if ( abs( n - raised ) < n ) {
      long rem = n - ( 1 * raised );
      // REPORT( "1" );
      return mergeSNAFU( "1", ltoSNAFU( rem ), exp );
    } else {
      cerr << "Unexpected (n > 0)" << endl;
      REPORT( n );
      exit( 1 );
    }
  }
}

static long SNAFUtol( string snafu )
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
        REPORT( (int)snafu[ 0 ] );
        exit( 1 );
    }
  }

  return 5 * SNAFUtol( snafu.substr( 0, snafu.size() - 1 ) )
        + SNAFUtol( snafu.substr( snafu.size() - 1, 1 ) );
}

int main()
{
  long total = 0;

  string line;
  while ( getline( cin, line ) ) {  
    // REPORT( line );
    long inputN = SNAFUtol( line );     // e = expected
    // REPORT( inputN );
    string lina = ltoSNAFU( inputN );  // a = actual
    if ( line != lina ) {
      REPORT( inputN );
      REPORT( line );
      REPORT( lina );
      REPORT( SNAFUtol( lina ) );
      exit( 1 );
    }
    total += inputN;
  }
  REPORT( total );
  string snafu = ltoSNAFU( total );
  long n = SNAFUtol( snafu );
  if ( total != n ) {
    REPORT( total );
    REPORT( n );
    REPORT( snafu );
    exit( 1 );
  }

  REPORT( snafu );
}
