#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

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


int main()
{
  string line;
  
  int idx = 0;
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

  for ( int i = 0; i < idx; i++ ) {
    cout << i << endl;
    for ( auto a : *monkeys[ i ]->items ) {
      cout << a << " ";
    }
    cout << endl;
  }
}
