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

// Aliasing
#define forn( X ) \
for ( int ii = 0; ii < ( X ); ii++ )

typedef struct {
  string name;
  int rate;
  vector<string> list;
} Valve;

// Only for parsing
vector<Valve*> parsingList;

// name lookup
map<string,Valve*> valves;

typedef pair<string,string> ps;

// Note: distance is bi-directional
map<ps,int> distance;

static Valve* parseLine( string line )
{
  // Make the valve
  Valve *valve = new Valve;

  // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
  stringstream ss( line );

  string discardStr;
  char discardChar = '\0';

  // Valve
  forn( 1 )
    ss >> discardStr;
  // AA
  ss >> valve->name;
  // has flow
  forn( 2 )
    ss >> discardStr; 
  // rate=
  forn( 5 )
    ss >> discardChar;
  //0
  ss >> valve->rate;
  // ; tunnels lead to valves
  forn( 5 )
    ss >> discardStr;
  // Fill it in
  char ch1, ch2;
  while( ss >> ch1 && ss >> ch2 ) {
    string name;
    name += ch1;
    name += ch2;
    valve->list.push_back( name );
    // Maybe comma
    ss >> discardChar;
  }

  return valve;
}

static void printValves( FILE *fp )
{
  for ( Valve* valve : parsingList ) {
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    if ( valve->list.size() == 1 ) {
      fprintf( fp, "Valve %s has flow rate=%d; tunnel leads to valve %s\n", valve->name.c_str(), valve->rate, valve->list[ 0 ].c_str() );
      continue;
    }
  
    fprintf( fp, "Valve %s has flow rate=%d; tunnels lead to valves ", valve->name.c_str(), valve->rate );
    int i;
    for ( i = 0; i < valve->list.size() - 1; i++ )
      fprintf( fp, "%s, ", valve->list[ i ].c_str() );
    fprintf( fp, "%s\n", valve->list[ i ].c_str() );
  }
}

int main()
{
  string line;

  while ( getline( cin, line ) ) {
    Valve *valve = parseLine( line );
    valves[ valve->name ] = valve;
    parsingList.push_back( valve );
  }

  // Check parsing
  FILE *parse = fopen( "PARSE", "w" );
  printValves( parse );
  fclose( parse );
}
