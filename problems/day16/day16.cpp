#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <queue>

#include <cstdio>

using namespace std;

#define REPORT( X ) cout << #X << " = " << ( X ) << endl

// like echo -n
#define REPORTN( X ) cout << #X << " = " << ( X ) << ", "

#define forn( X ) \
for ( int i = 0; i < ( X ); i++ )

typedef struct ValveTag {
  string name;
  int rate;
  vector<string> *list;
} Valve;

map<string,Valve*> valves;

int totals;

int pathCount = 0;

int minuteMax = 30;

// What needs to be queued for bfs
typedef struct {
  string current;
  set<Valve*> openValves;
  set<string> visited;
  int total;
} State;

/**
 * Simulate brute-force
 * 
 * @param current current valve
 * @param minute time in minutes
 * @param total total flow
 * @param openValves open valves
 * @param visited nodes visited since the last open valve
 */
static int simulate( string current, int minute, int total, set<Valve*> openValves, vector<string> path, set<string> visited )
{
  State state;
  state.current = current;
  state.openValves = openValves;
  state.visited = visited;
  state.total = total;

  queue<State> q;
  q.push( state );

  while ( !q.empty() ) {
    size_t size = q.size();
    for ( size_t i = 0; i < size; i++ ) {
      State state = q.front();
      q.pop();

      current = state.current;
      openValves = state.openValves;
      visited = state.visited;
      total = state.total;

      // Add from previous
      int releasing = 0;
      for ( Valve *valve : openValves )
        releasing += valve->rate;

      if ( openValves.size() == valves.size() ) {
        REPORT( openValves.size() );
        total += ( minuteMax - minute + 1 ) * releasing;
        return total;
      } else {
        total += releasing;
      }

      path.push_back( current );

      // cout << endl;

      // Done
      if ( minute >= minuteMax ) {
        // for ( string str : path )
        //   cout << str << ", ";
        // cout << endl;
        if ( total > totals )
          totals = total;
        REPORT( total );
        continue;
      }

      // Prune out paths that just do nothing
      if ( ! visited.insert( current ).second )
        continue;

      // Get the current valve
      Valve *valve = valves[ current ];

      // Try all possiblities
      if ( openValves.count( valve ) == 1 ) { // already open
        for ( string name : *valve->list ) {
          State state;
          state.current = name;
          state.total = total;
          state.openValves = openValves;
          state.visited = visited;

          // simulate( name, minute + 1, total, openValves, path, visited );

          q.push( state );
        }
      } else {
        // Skip it
        for ( string name : *valve->list ) {
          State state;
          state.current = name;
          state.total = total;
          state.openValves = openValves;
          state.visited = visited;

          // simulate( name, minute + 1, total, openValves, path, visited );

          q.push( state );
        }
        
        // Open it
        openValves.insert( valve );
        visited.clear();

        State state;
        state.current = current;
        state.total = total;
        state.openValves = openValves;
        state.visited = visited;
    
        // simulate( current, minute + 1, total, openValves, path, visited );

        q.push( state );
      }
    }

    minute++;
  }
  return -1;
}

int main( int argc, char *argv[] )
{
  if ( argc == 2 )
    minuteMax = atoi ( argv[ 1 ] );

  string line;
    
  while ( getline( cin, line ) ) {
    // Make the valve
    Valve *valve;
    valve = (Valve *)malloc( sizeof( *valve ) );

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
    valve->list = new vector<string>;
    char ch1, ch2;
    while( ss >> ch1 && ss >> ch2 ) {
      string name;
      name += ch1;
      name += ch2;
      valve->list->push_back( name );
      // Maybe comma
      ss >> discardChar;
    }

    valves[ valve->name ] = valve;
  }

  // Check parsing
  REPORT( valves.size() );
  FILE *parse = fopen( "PARSE", "w" );
  for ( auto itemPair : valves ) {
    auto valve = itemPair.second;
    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    fprintf( parse, "Valve %s has flow rate=%d; tunnels lead to valves ", valve->name.c_str(), valve->rate );
    int i;
    for ( i = 0; i < valve->list->size() - 1; i++ )
      fprintf( parse, "%s, ", (*valve->list)[ i ].c_str() );
    fprintf( parse, "%s\n", (*valve->list)[ i ].c_str() );
  }
  fclose( parse );

  // Ignite
  int minute = 1;
  int total = 0;
  set<Valve*> openValves;
  vector<string> path;
  set<string> visited;

  // No point opening these anyways
  for ( auto item : valves ) {
    if ( item.second->rate == 0 )
      openValves.insert( item.second );
  }

  cout << simulate( "AA", minute, total, openValves, path, visited ) << endl;
}
