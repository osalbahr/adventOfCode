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
map<ps,int> distances;

// Subset of valves, only the important ones
map<string,Valve*> usefulValves;

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

// Using bfs
static void populateDistances()
{
  for ( const auto& item : valves ) {
    Valve *valve = item.second;
    string name = valve->name;
    int rate = valve->rate;

    if ( rate == 0 && name != "AA" )
      continue;

    int distance = 0;
    queue<Valve*> q;
    set<string> visited;
    for ( string otherString : valve->list ) {
      q.push( valves[ otherString ] );
      visited.insert( otherString );
    }

    visited.insert( name );
    
    while ( !q.empty() ) {
      distance++;
      int size = q.size();
      for ( int i = 0; i < size; i++ ) {
        Valve* other = q.front();
        q.pop();

        string otherString = other->name;
        if ( usefulValves.count( otherString ) > 0 ) {
          ps p = { name, otherString };
          ps p2 = { otherString, name };
          distances[ p ] = distances[ p2 ] = distance;
        }

        for ( string nextString : other->list ) {
          // Newly inserted
          if ( visited.insert( nextString ).second )
            q.push( valves[ nextString ] );
        }
      }
    }
  }
}

static set<string> getPaths()
{
  set<string> allPaths;
  for ( auto item : usefulValves )
    allPaths.insert( item.second->name );
  
  int size = usefulValves.size();
  for ( int i = 0; i < size - 1; i++ ) {
    set<string> allPathsCopy = allPaths;
    allPaths.clear();
    for ( auto item : usefulValves )
      for ( string path : allPathsCopy ) {
        string name = item.second->name;
        bool duplicate = false;
        for ( int i = 0; i < path.size() - 1; i += 2 )
          if ( path[ i ] == name[ 0 ] && path[ i + 1 ] == name[ 1 ] ) {
            duplicate = true;
            break;
          }

        if ( !duplicate ) {
          string newPath = path + name;
          allPaths.insert( newPath );
        }
      }
  }
  REPORT( allPaths.size() );
  for ( string path : allPaths )
    REPORT( path );
  REPORT( "Done" );
  exit( 0 );

  set<string> paths;
  return paths;
}

static vector<int> getFlowRates()
{
  Valve *start = valves[ "AA" ];
  int minutes = 0;
  set<string> paths = getPaths();
}

int main()
{
  string line;

  while ( getline( cin, line ) ) {
    Valve *valve = parseLine( line );
    valves[ valve->name ] = valve;
    if ( valve->rate > 0 )
      usefulValves[ valve->name ] = valve;
    parsingList.push_back( valve );
  }

  // Check parsing
  FILE *parse = fopen( "PARSE", "w" );
  printValves( parse );
  fclose( parse );
  REPORT( valves.size() );
  REPORT( usefulValves.size() );

  populateDistances();

  REPORT( distances.size() );

  FILE *graph = fopen( "GRAPH", "w" );
  for ( auto item : distances ) {
    ps p = item.first;
    string s1 = p.first;
    string s2 = p.second;
    int dist = item.second;
    if ( s1 < s2 )
      fprintf( graph, "%s <- %d -> %s\n", s1.c_str(), dist, s2.c_str() );
  }
  fclose( graph );

  vector<int> rates = getFlowRates();
  sort( rates.begin(), rates.end() );
  reverse( rates.begin(), rates.end() );
  cout << rates[ 0 ] << endl;
}
