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
  vector<int> *items;
  string *operation;
  int divideTest;
  int trueIdx;
  int falseIdx;
} Monkey;

Monkey **monkeys = (Monkey **)malloc( 8 * sizeof( Monkey ) );

// Where to insert
int idx = 0;

// Keep track of inspections here
vector<int> inspections( 8 );

static int applyOperation( int worryLevel, string operation )
{
  int operand;
  if ( operation[ 1 ] == 'o' ) {
    operand = worryLevel;
  } else {
    sscanf( operation.c_str() + 1, "%d", &operand );
  }

  if ( operation[ 0 ] == '+' ) {
    worryLevel += operand;
  } else {
    worryLevel *= operand;
  }

  return worryLevel;
}

static void doRound()
{
  for ( int i = 0; i < idx; i++ ) {
    // cout << "Monkey " << i << endl;

    Monkey *monkey = monkeys[ i ];
    vector<int> *items = monkey->items;
    inspections[ i ] += items->size();
    cout << inspections[ i ] << endl;
    for ( auto worryLevel : *items ) {

      // cout << "Item " << worryLevel << endl;;
      worryLevel = applyOperation( worryLevel, *monkey->operation );
      // cout << "After " << worryLevel << endl;

      worryLevel /= 3;
      // cout << "Bored " << worryLevel << endl;

      // cout << "True idx: " << monkey->trueIdx << endl;
      int throwIdx = worryLevel % monkey->divideTest == 0 ?
                     monkey->trueIdx :
                     monkey->falseIdx;

      // cout << "Throwing to " << throwIdx << endl;
      monkeys[ throwIdx ]->items->push_back( worryLevel );
    }
    items->clear();

    cout << endl;
  }
}

int main()
{
  string line;
  
  while ( getline( cin, line ) ) {
    stringstream ss( line );
    Monkey *monkey = (Monkey *)malloc( sizeof( Monkey ) );
    monkey->idx = idx;
    monkey->items = new vector<int>();
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


  int rounds = 20;
  for ( int round = 1; round <= rounds; round++ ) {
    doRound();

    cout << "Round summary: " << round << endl;
    for ( int i = 0; i < idx; i++ ) {
      cout << "Monkey " << i << ": ";
      for ( int item : *monkeys[ i ]->items )
        cout << item << ", ";
      cout << endl;
    }
  }

  // Find top 2
  sort( inspections.begin(), inspections.end() );
  reverse( inspections.begin(), inspections.end() );
  cout << inspections[ 0 ] * inspections[ 1 ] << endl;
}
