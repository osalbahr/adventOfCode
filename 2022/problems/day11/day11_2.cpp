#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>

#include <cstdio>

using namespace std;

typedef struct MonketTag {
  int idx;
  vector<long> *items;
  string *operation;
  int divideTest;
  int trueIdx;
  int falseIdx;
} Monkey;

Monkey **monkeys = (Monkey **)malloc( 8 * sizeof( Monkey ) );

// Where to insert
int idx = 0;

// Keep track of inspections here
vector<long> inspections( 8 );

// To reduce big numbers
int reducer = 1;

static long applyOperation( long worryLevel, string operation )
{
  worryLevel %= reducer;
  // cout << worryLevel << endl;
  // cout << operation << endl;
  long operand;
  if ( operation[ 1 ] == 'o' ) {
    operand = worryLevel;
  } else {
    sscanf( operation.c_str() + 1, "%ld", &operand );
  }

  if ( operation[ 0 ] == '+' ) {
    worryLevel += operand;
  } else {
    worryLevel *= operand;
  }

  // cout << worryLevel << endl;

  if ( worryLevel < 0 )
    exit( 1 );

  worryLevel %= reducer;
  return worryLevel;
}

static void doRound()
{
  for ( int i = 0; i < idx; i++ ) {
    // cout << "Monkey " << i << endl;

    Monkey *monkey = monkeys[ i ];
    vector<long> *items = monkey->items;
    inspections[ i ] += items->size();
    // // cout << inspections[ i ] << endl;
    for ( auto worryLevel : *items ) {

      // // cout << "Item " << worryLevel << endl;;
      worryLevel = applyOperation( worryLevel, *monkey->operation );
      // // cout << "After " << worryLevel << endl;


      // // cout << "True idx: " << monkey->trueIdx << endl;
      int throwIdx = worryLevel % monkey->divideTest == 0 ?
                     monkey->trueIdx :
                     monkey->falseIdx;

      // // cout << "Throwing to " << throwIdx << endl;
      monkeys[ throwIdx ]->items->push_back( worryLevel );

      // // cout << "Inner items: " << endl;
      // for ( int i = 0; i < idx; i++ ) {
      //   // cout << "Monkey " << i << ": ";
      //   for ( int item : *monkeys[ i ]->items )
      //     // cout << item << ", ";
      //   // cout << endl;
      // }

    }
    items->clear();
  }
}

int main()
{
  string line;
  
  while ( getline( cin, line ) ) {
    stringstream ss( line );
    Monkey *monkey = (Monkey *)malloc( sizeof( Monkey ) );
    monkey->idx = idx;
    monkey->items = new vector<long>();
    monkey->operation = new string();

    // Items
    getline( cin, line );
    stringstream items( line );
    
    // Discard first two words
    items >> line >> line;
    int item;
    while ( items >> item ) {
      monkey->items->push_back( item );
      // Comma
      items >> line;
    }

    // Operation
    getline( cin, line );
    stringstream operation( line );


    // Discard Operation: new = old
    operation >> line >> line >> line >> line;

    operation >> *monkey->operation;
    operation >> line;
    *monkey->operation += line;

    // Test
    getline( cin, line );
    stringstream test( line );

    // Discard "Test: divisible by"
    test >> line >> line >> line;
    test >> monkey->divideTest;

    // True path
    getline( cin, line );
    stringstream trueString( line );
    // Keep discarding
    for ( int i = 0; i < 5; i++ )
      trueString >> line;

    trueString >> monkey->trueIdx;

    // False path
    getline( cin, line );
    stringstream falseString( line );
    // Keep discarding
    for ( int i = 0; i < 5; i++ )
      falseString >> line;

    falseString >> monkey->falseIdx;

    // There might be an empty line
    getline( cin, line );

    // Insert monkey
    monkeys[ idx ] = monkey;
    idx++;
  }

  for ( int i = 0; i < idx; i++ ) {
    reducer *= monkeys[ i ]->divideTest;
  }
  // cout << reducer << endl;


  int rounds = 10000;
  for ( int round = 1; round <= rounds; round++ ) {
    doRound();

    // // cout << "Round summary: " << round << endl;
    // for ( int i = 0; i < idx; i++ )
    //   // cout << "Monkey " << i << ": " << inspections[ i ] << endl;
    
    // // cout << "Round items: " << round << endl;
    // for ( int i = 0; i < idx; i++ ) {
    //   // cout << "Monkey " << i << ": ";
    //   for ( int item : *monkeys[ i ]->items )
    //     // cout << item << ", ";
    //   // cout << endl;
    // }

  }

  // Find top 2
  sort( inspections.begin(), inspections.end() );
  reverse( inspections.begin(), inspections.end() );
  cout << inspections[ 0 ] * inspections[ 1 ] << endl;
}
