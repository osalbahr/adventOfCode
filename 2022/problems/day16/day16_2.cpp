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

#define TIME_LIMIT 26

// For debugging
#define REPORT( X ) cout << #X " = " << X << endl
// like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "

// Aliasing
#define forn( X ) \
for ( int ii = 0; ii < ( X ); ii++ )

typedef struct Valve {
  string name;
  int rate;
  // vector<string> list;
} Valve;
unordered_map<string,vector<string>> valveList;

// Adapted from
// https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
bool operator<(const Valve& x, const Valve& y) {
    return x.name < y.name;
}

// Only for parsing
vector<Valve> parsingList;

// name lookup
unordered_map<string,Valve> valves;

typedef pair<string,string> ps;

// Adapted from
// https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
// Only for pairs of long
// You can of course template this struct to allow other hash functions
struct ps_hash {
    long operator () (const ps &p) const {
        auto hasher = hash<string>{};
        // Not cryptographically secure, I know
        return hasher(p.first) ^ hasher(p.second);  
    }
};

// Note: distance is bi-directional
unordered_map<ps,int,ps_hash> distances;

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
    valveList[ valve.name ].push_back( name );
    // Maybe comma
    ss >> discardChar;
  }

  return valve;
}

static void printValves( FILE *fp )
{
  for ( Valve valve : parsingList ) {
    auto list = valveList[ valve.name ];
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    if ( list.size() == 1 ) {
      fprintf( fp, "Valve %s has flow rate=%d; tunnel leads to valve %s\n", valve.name.c_str(), valve.rate, list[ 0 ].c_str() );
      continue;
    }
  
    fprintf( fp, "Valve %s has flow rate=%d; tunnels lead to valves ", valve.name.c_str(), valve.rate );
    size_t i;
    for ( i = 0; i < list.size() - 1; i++ )
      fprintf( fp, "%s, ", list[ i ].c_str() );
    fprintf( fp, "%s\n", list[ i ].c_str() );
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
    unordered_set<string> visited;
    for ( string otherString : valveList[ valve.name ] ) {
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

        for ( string nextString : valveList[ other.name ] ) {
          // Newly inserted
          if ( visited.insert( nextString ).second )
            q.push( valves[ nextString ] );
        }
      }
    }
  }
}

unordered_map<string,int> memoizeTimes;
static int getTime( const string& path )
{
  if ( path.size() == 4 )
    return distances[ { path.substr( 0, 2 ), path.substr( 2, 2 ) } ] + 1;
  
  if ( memoizeTimes.count( path ) > 0 )
    return memoizeTimes[ path ];

  string main = path.substr( 0, path.size() - 2 );
  string other = path.substr( path.size() - 4, 4 );
  return memoizeTimes[ path ] =
    getTime( main ) + getTime( other );
}

static unordered_set<string> getPaths( const map<string,Valve>& usefulValves )
{
  // cout << "Start ... " << flush;
  unordered_set<string> shortPaths;
  unordered_set<string> allPaths;
  for ( auto item : usefulValves ) {
    string newPath = "AA" + item.second.name;
    if ( getTime( newPath ) < TIME_LIMIT )
      allPaths.insert( newPath );
  }
  
  int size = usefulValves.size();
  for ( int i = 0; i < size - 1; i++ ) {
    // REPORT( allPaths.size() );
    unordered_set<string> allPathsCopy = allPaths;
    unordered_set<string> toBeRemoved;
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
        if ( !duplicate && getTime( newPath ) < TIME_LIMIT ) {
          allPaths.insert( newPath );
          toBeRemoved.insert( path );
        }
      }
    }

    if ( toBeRemoved.empty() ) {
      // cout << "PRIMED at i = " << i << " " << flush;
      break;
    }

    // For part 2 we actually want to keep them for later
    for ( string removal : toBeRemoved ) {
      allPaths.erase( removal );
      shortPaths.insert( removal );
    }
  }

  allPaths.insert( shortPaths.begin(), shortPaths.end() );

  // REPORT( allPaths.size() );
  return allPaths;
}

typedef struct {
  int rate;
  int throughput;
  int minute;
} RateInfo;
unordered_map<string,RateInfo> memoizeRates;

// set<Valve> operator+( set<Valve>& lhs, const set<Valve>& rhs ) {
//   lhs.insert( rhs.begin(), rhs.end() );
//   return lhs;
// }

static RateInfo getRateHelper( const string& path )
{
  if ( memoizeRates.count( path ) > 0 ) {
    // cout << "HIT " << path << "\n";
    return memoizeRates[ path ];
  }
  // cout << "MISS " << path << "\n";

  if ( path.size() == 4 ) {
    RateInfo rateInfo;
    Valve valve = usefulValves[ path.substr( 2, 2 ) ];
    rateInfo.rate = 0;
    rateInfo.throughput = valve.rate;
    rateInfo.minute = getTime( path );
    return memoizeRates[ path ] =
      rateInfo;
  }

  string oldString = path.substr( 0, path.size() - 2 );
  string newString = path.substr( path.size() - 4, 4 );
  RateInfo oldInfo = memoizeRates[ oldString ]
    = getRateHelper( oldString );
  RateInfo newInfo  = memoizeRates[ newString ]
    = getRateHelper( newString );
  int time = newInfo.minute;
  return memoizeRates[ path ]
    = {
        oldInfo.rate + time * oldInfo.throughput,
        oldInfo.throughput + newInfo.throughput,
        oldInfo.minute + time
      };
}

static int getRate( string path )
{
  RateInfo rateInfo = getRateHelper( path );
  int rate = rateInfo.rate;
  int throughput = rateInfo.throughput;
  int minute = rateInfo.minute;

  // Leftover profit
  rate += ( TIME_LIMIT - minute ) * throughput;
  return rate;
}

unordered_map<string,int> elephantRates;
static int getRateElephant( string path )
{
  int rate = 0;
  map<string,Valve> copyValves = usefulValves;

  // Get the paths of the compliment
  for ( int i = 2; i < (signed)path.size() - 1; i += 2 ) {
    copyValves.erase( path.substr( i, 2 ) );
  }
  string elephantString;
  for ( auto& [ key, val ] : copyValves )
    elephantString += key;

  if ( elephantRates.count( elephantString ) > 0 )
    return elephantRates[ elephantString ];

  auto paths = getPaths( copyValves );

  // Get the max rate
  for ( string path : paths ) {
    rate = max( rate, getRate( path ) );
  }

  return elephantRates[ elephantString ] = rate;
}

static vector<int> getFlowRates()
{
  cout << "Generating PATHSFILE ... " << flush;
  unordered_set<string> paths = getPaths( usefulValves );
  FILE *pathsfile = fopen( "PATHSFILE", "w" );
  for ( string path : paths ) {
    fprintf( pathsfile, "%s\n", path.c_str() );
  }
  fclose( pathsfile );

  cout << "Done" << endl;
  REPORT( paths.size() );
  REPORT( memoizeTimes.size() );

  vector<int> rates;
  int i = 0;
  for ( string path : paths ) {
    if ( i++ % 1000 == 0 )
      REPORT( i - 1 );
    int person = getRate( path );
    int elephant = getRateElephant( path );
    int total = person + elephant;
    rates.push_back( total );
    // cout << person << " + " << elephant << " = " << total << endl;
  }
  REPORT( memoizeRates.size() );
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

  vector<int> rates = getFlowRates();
  sort( rates.begin(), rates.end() );
  cout << "Min = " << rates[ 0 ] << endl;
  cout << "Max = " << rates[ rates.size() - 1 ] << endl;
}
