// For https://www.reddit.com/r/cpp_questions/comments/101f70k/how_to_generate_x_random_integers_whose_sum_is_100

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>    // stringstream
#include <algorithm>  // sort
#include <queue>
#include <stack>
#include <numeric>

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



static set<vector<int>> generate( int n )
{
  if ( n == 1 )
    return { { 100 } };

  set<vector<int>> allOrders;

  // Insert 0 to 100
  for ( int number = 0; number <= 100; number++ ) {
    allOrders.insert( { number } );
  }

  // Append n-2 times
  for ( int i = 0; i < n - 2; i++ ) {
    auto oldOrders = allOrders;
    allOrders.clear();
    for ( auto order : oldOrders ) {
      int sum = accumulate( order.begin(), order.end(), 0 );
      // Append without overflowing
      for ( int number = 0; number <= 100 - sum; number++ ) {
        auto newOrder = order;
        newOrder.push_back( number );
        allOrders.insert( newOrder );
      }
    }
  }

  // Special last append
  set<vector<int>> ret;
  for ( auto order : allOrders ) {
    int sum = accumulate( order.begin(), order.end(), 0 );
    order.push_back( 100 - sum );
    ret.insert( order );
  }
  return ret;
}

static void printNumbers( const set<vector<int>>& orders, FILE *fp )
{
  for ( const vector<int>& numbers : orders ) {
    int i;
    for ( i = 0; i < numbers.size() - 1; i++ ) {
      fprintf( fp, "%d,", numbers[ i ] );
    }
    fprintf( fp, "%d\n", numbers[ i ] );
  }
}

int main()
{
  for ( int n = 1; n <= 5; n++ ) {
    set<vector<int>> orders = generate( n );
    string filename("GENERATEFILE");
    filename += '0' + n;
    // REPORT( filename );
    FILE *out = fopen( filename.c_str(), "w" );
    printNumbers( orders, out );
    fclose( out );
  }
}
