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
    size_t i;
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

map<string,int> memoizeTimes;
static int getTime( string path )
{
  if ( path.size() == 2 ) {
    return memoizeTimes[ path ] = distances[ { "AA", path.substr( 2, 2 ) } ] + 1;
  } else {
    string lastTwo = path.substr( path.size() - 4, 4 );
    ps pairString = { lastTwo.substr( 0, 2 ), lastTwo.substr( 2, 2 ) };
    return memoizeTimes[ path ] = memoizeTimes[ path ] + distances[ pairString ] + 1;
  }
}

static bool duplicate( string path, string newValve ) {
  for ( int i = 2; i < path.size() - 1; i += 2 ) {
    if ( path.substr( i, 2 ) == newValve )
      return true;
  }
  return false;
}

static void assertPS( ps pairString )
{
  return;

  string str1 = pairString.first;
  string str2 = pairString.second;
  if ( str1 >= str2 ) {
    REPORT( str1 );
    REPORT( str2 );
    exit( 1 );
  }

  for ( int i = 2; i < str1.size(); i += 2 )
    for ( int j = 2; j < str2.size(); j += 2 )
      if ( str1.substr( i, 2 ) == str2.substr( j, 2 ) ) {
        REPORT( str1 );
        REPORT( str2 );
        exit( 1 );
      }
}

static set<ps> getPaths()
{
  set<ps> allPaths;

  // Double ignite
  for ( auto item : usefulValves ) {
    string newPath = "AA" + item.first;
    if ( getTime( newPath ) >= 26 )
      continue;
    bool done = false;
    for ( auto item2 : usefulValves ) {
      string newPath2 = "AA" + item2.first;
      if ( newPath == newPath2 || getTime( newPath2 ) >= 26 )
        continue;
      if ( newPath < newPath2 )
        allPaths.insert( { newPath, newPath2 } );
      else
        done = true;
    }
    if ( done )
      break;
  }
  assert( allPaths.size() );
  for ( ps pairString : allPaths ) {
    assertPS( pairString );
  }
  
  int size = usefulValves.size();
  for ( int i = 0; i < size - 1; i++ ) {
    REPORT( i );
    REPORT( allPaths.size() );
    set<ps> allPathsCopy = allPaths;
    REPORT( allPathsCopy.size() );
    set<ps> toBeRemoved;
    for ( auto item : usefulValves ) {
      for ( ps pairString : allPathsCopy ) {
        assertPS( pairString );
        string path1 = pairString.first;
        string path2 = pairString.second;
        string name = item.second->name;
        bool duplicate1 = duplicate( path1, name );
        bool duplicate2 = duplicate( path2, name );

        string newPath1 = path1 + name;
        if ( !duplicate1 && !duplicate2 && getTime( newPath1 ) < 26 ) {
          newPath1 < path2
          ? allPaths.insert( { newPath1, path2 } )
          : allPaths.insert( { path2, newPath1 } );
          toBeRemoved.insert( pairString );
        }

        string newPath2 = path2 + name;
        if ( !duplicate2 && !duplicate1 && getTime( newPath2 ) < 26 ) {
          path1 < newPath2
          ? allPaths.insert( { path1, newPath2 } )
          : allPaths.insert( { newPath2, path1 } );
          toBeRemoved.insert( pairString );
        }
      }
    }

    if ( toBeRemoved.empty() ) {
      cout << "PRIMED at ";
      REPORT( i );
      break;
    }
  
    for ( ps removal : toBeRemoved )
      allPaths.erase( removal );
  }

  FILE *pathsfile = fopen( "PATHSFILE", "w" );
  for ( ps paths : allPaths ) {
    fprintf( pathsfile, "%s:%s\n", paths.first.c_str(), paths.second.c_str() );
  }
  fclose( pathsfile );

  return allPaths;
}

static int getRate( string path )
{
  if ( path == "AA" )
    return 0;

  set<Valve*> openValves;

  int rate = 0;
  int throughput = 0;
  int minute = 0;
  for ( int i = 0; i < path.size() - 3; i += 2 ) {
    string src = path.substr( i, 2 );
    string openvalve = path.substr( i + 2, 2 );

    // Go and open it
    int time = distances[ { src, openvalve } ] + 1;

    minute += time;

    // Old profit
    rate += time * throughput;

    // New profit
    throughput += usefulValves[ openvalve ]->rate;
  }

  // Leftover profit
  rate += ( 26 - minute ) * throughput;
  return rate;
}

static int getRateBoth( string str1, string str2 )
{
  return getRate( str1 ) + getRate( str2 );
}

static vector<int> getFlowRates()
{
  cout << "Generating PATHSFILE ... " << flush;
  set<ps> paths = getPaths();
  cout << "Done" << endl;
  REPORT( paths.size() );

  cout << "Generating pathsRates ... " << flush;
  map<string,int> pathsRates;
  for ( ps pathsPair : paths ) {
    string str1 = pathsPair.first;
    string str2 = pathsPair.second;
    int rate1 = getRate( str1 );
    int rate2 = getRate( str2 );
    pathsRates[ str1 ] = rate1;
    pathsRates[ str2 ] = rate2;
  }
  cout << endl;
  REPORT( pathsRates.size() );

  cout << "Generating rates ... " << flush;
  vector<int> rates;
  for ( ps pathsPair : paths ) {
    string str1 = pathsPair.first;
    string str2 = pathsPair.second;
    int total = pathsRates[ str1 ] + pathsRates[ str2 ];
    rates.push_back( total );
    if ( total > 1707 ) {
      cout << endl;
      REPORTN( str1 ), REPORTN( str2 ), REPORT( total );
      REPORT( getTime( str1 ) );
      REPORT( getTime( str2 ) );
      exit( 1 );
    }
  }
  cout << "Done" << endl;
  REPORT( rates.size() );

  return rates;
}

int main()
{
  string line;

  cout << "Parsing ... ";
  cout.flush();
  while ( getline( cin, line ) ) {
    Valve *valve = parseLine( line );
    valves[ valve->name ] = valve;
    if ( valve->rate > 0 )
      usefulValves[ valve->name ] = valve;
    parsingList.push_back( valve );
  }
  cout << "Done" << endl;

  // Check parsing
  cout << "Creating PARSE ... " << flush;
  FILE *parse = fopen( "PARSE", "w" );
  printValves( parse );
  fclose( parse );
  cout << "Done" << endl;

  cout << "Populating distance ... " << flush;
  populateDistances();
  cout << "Done" << endl;

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
  assert( rates.size() );
  sort( rates.begin(), rates.end() );
  cout << "Min = " << rates[ 0 ] << endl;
  reverse( rates.begin(), rates.end() );
  cout << "Max = " << rates[ 0 ] << endl;

  // string path = "AAZBLZRECYJFIU";
  // int rate = getRate( path );

  // REPORT( path );
  // REPORT( rate );

  string you = "AAJJBBCC";
  string elephant = "AADDHHEE";
  REPORT( getRateBoth( you, elephant ) );
}
