#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>    // stringstream
#include <algorithm>  // sort
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

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
vector<Valve> parsingList;

// name lookup
map<string,Valve> valves;

typedef pair<string,string> ps;

// Note: distance is bi-directional
map<ps,int> distances;

// Subset of valves, only the important ones
map<string,Valve> usefulValves;

static Valve parseLine( string line )
{
  // Make the valve
  Valve valve;

  // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
  stringstream ss( line );

  string discardStr;
  char discardChar = '\0';

  // Valve
  forn( 1 )
    ss >> discardStr;
  // AA
  ss >> valve.name;
  // has flow
  forn( 2 )
    ss >> discardStr; 
  // rate=
  forn( 5 )
    ss >> discardChar;
  //0
  ss >> valve.rate;
  // ; tunnels lead to valves
  forn( 5 )
    ss >> discardStr;
  // Fill it in
  char ch1, ch2;
  while( ss >> ch1 && ss >> ch2 ) {
    string name;
    name += ch1;
    name += ch2;
    valve.list.push_back( name );
    // Maybe comma
    ss >> discardChar;
  }

  return valve;
}

static void printValves( FILE *fp )
{
  for ( Valve valve : parsingList ) {
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    if ( valve.list.size() == 1 ) {
      fprintf( fp, "Valve %s has flow rate=%d; tunnel leads to valve %s\n", valve.name.c_str(), valve.rate, valve.list[ 0 ].c_str() );
      continue;
    }
  
    fprintf( fp, "Valve %s has flow rate=%d; tunnels lead to valves ", valve.name.c_str(), valve.rate );
    size_t i;
    for ( i = 0; i < valve.list.size() - 1; i++ )
      fprintf( fp, "%s, ", valve.list[ i ].c_str() );
    fprintf( fp, "%s\n", valve.list[ i ].c_str() );
  }
}

// Using bfs
static void populateDistances()
{
  for ( const auto& item : valves ) {
    Valve valve = item.second;
    string name = valve.name;
    int rate = valve.rate;

    if ( rate == 0 && name != "AA" )
      continue;

    int distance = 0;
    queue<Valve> q;
    set<string> visited;
    for ( string otherString : valve.list ) {
      q.push( valves[ otherString ] );
      visited.insert( otherString );
    }

    visited.insert( name );
    
    while ( !q.empty() ) {
      distance++;
      int size = q.size();
      for ( int i = 0; i < size; i++ ) {
        Valve other = q.front();
        q.pop();

        string otherString = other.name;
        if ( usefulValves.count( otherString ) > 0 ) {
          ps p = { name, otherString };
          ps p2 = { otherString, name };
          distances[ p ] = distances[ p2 ] = distance;
        }

        for ( string nextString : other.list ) {
          // Newly inserted
          if ( visited.insert( nextString ).second )
            q.push( valves[ nextString ] );
        }
      }
    }
  }
}

unordered_map<string,int> memoizeTimes;
static int getTime( string path )
{
  if ( path.size() == 4 )
    return distances[ { path.substr( 0, 2 ), path.substr( 2, 2 ) } ] + 1;
  
  if ( memoizeTimes.count( path ) > 0 )
    return memoizeTimes[ path ];

  string main = path.substr( 0, path.size() - 2 );
  string other = path.substr( path.size() - 4, 4 );
  return memoizeTimes[ path ] = getTime( main ) + getTime( other );
}

static unordered_set<string> getPaths()
{
  unordered_set<string> allPaths;
  for ( auto item : usefulValves ) {
    string newPath = "AA" + item.second.name;
    if ( getTime( newPath ) < 30 )
      allPaths.insert( newPath );
  }
  
  int size = usefulValves.size();
  for ( int i = 0; i < size - 1; i++ ) {
    // REPORT( allPaths.size() );
    unordered_set<string> allPathsCopy = allPaths;
    set<string> toBeRemoved;
    for ( auto item : usefulValves ) {
      for ( string path : allPathsCopy ) {
        string name = item.second.name;
        bool duplicate = false;
        for ( size_t i = 0; i < path.size() - 1; i += 2 )
          if ( path[ i ] == name[ 0 ] && path[ i + 1 ] == name[ 1 ] ) {
            duplicate = true;
            break;
          }

        string newPath = path + name;
        if ( !duplicate && getTime( newPath ) < 30 ) {
          allPaths.insert( newPath );
          toBeRemoved.insert( path );
        }
      }
    }

    if ( toBeRemoved.empty() ) {
      cout << "PRIMED at i = " << i << " " << flush;
      break;
    }
  
    for ( string removal : toBeRemoved )
      allPaths.erase( removal );
  }

  FILE *pathsfile = fopen( "PATHSFILE", "w" );
  for ( string path : allPaths ) {
    fprintf( pathsfile, "%s\n", path.c_str() );
  }
  fclose( pathsfile );

  return allPaths;
}

static int getRate( string path )
{
  set<Valve> openValves;

  int rate = 0;
  int throughput = 0;
  int minute = 0;

  for ( size_t i = 0; i < path.size() - 3; i += 2 ) {
    string sourceValve = path.substr( i, 2 );
    string valveToOpen = path.substr( i + 2, 2 );

    // Go and open it
    int time = distances[ { sourceValve, valveToOpen } ] + 1;

    minute += time;

    // Old profit
    rate += time * throughput;

    // New profit
    throughput += usefulValves[ valveToOpen ].rate;
  }

  // Leftover profit
  rate += ( 30 - minute ) * throughput;
  return rate;
}

static vector<int> getFlowRates()
{
  cout << "Generating PATHSFILE ... " << flush;
  unordered_set<string> paths = getPaths();
  cout << "Done" << endl;
  REPORT( paths.size() );
  REPORT( memoizeTimes.size() );

  vector<int> rates;
  for ( string path : paths ) {
    int rate = getRate( path );
    rates.push_back( rate );
  }
  return rates;
}

int main()
{
  string line;

  cout.flush();
  while ( getline( cin, line ) ) {
    Valve valve = parseLine( line );
    valves[ valve.name ] = valve;
    if ( valve.rate > 0 )
      usefulValves[ valve.name ] = valve;
    parsingList.push_back( valve );
  }

  // Check parsing
  FILE *parse = fopen( "PARSE", "w" );
  printValves( parse );
  fclose( parse );

  populateDistances();

  cout << "Creating GRAPH ... " << flush;
  cout.flush();
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
  cout << "Done" << endl;

  vector<int> rates = getFlowRates();
  sort( rates.begin(), rates.end() );
  cout << "Min = " << rates[ 0 ] << endl;
  cout << "Max = " << rates[ rates.size() - 1 ] << endl;

  // for ( const auto& [ key, val ] : memoizeTimes ) {
  //   REPORT( key );
  // }
}
