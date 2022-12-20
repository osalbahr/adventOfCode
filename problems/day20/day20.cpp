#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

#define REPORT( X ) cout << #X << " = " << (X) << endl

typedef struct NodeTag {
  // struct NodeTag *prev;
  struct NodeTag *next;
  int n;
} Node;

typedef struct {
  // Node *tail;
  Node *head;
} List;

static void printList( List *list, FILE *fp )
{
  for ( Node *c = list->head; c; c = c->next )
    fprintf( fp, "%d\n", c->n );
}

// static void printListReverse( List *list, FILE *fp )
// {
//   for ( Node *c = list->tail; c; c = c->prev )
//     fprintf( fp, "%d\n", c->n );
// }

// static void mix( List *list )
// {
//   for ( Node *c = list->head; c; ) {
//     // Preserve the actual next
//   }
// }

int main()
{
  List *list = (List *)malloc( sizeof( *list ) );
  // same as calloc( 1, sizeof( *list ) )
  list->head = nullptr;
  // list->tail = nullptr;

  int n;
  cin >> n;
  Node *head;
  head = (Node *)malloc( sizeof( *list ) );
  head->n = n;
  // head->prev = nullptr;
  list->head = head;

  Node *prev = head;
  while ( cin >> n ) {
    Node *temp = (Node *)malloc( sizeof( *list ) );
    temp->n = n;
    prev->next = temp;
    // temp->prev = prev;
    prev = temp;
  }
  Node *tail = prev;
  tail->next = nullptr;
  // list->tail = tail;

  // Check it
  fclose( fopen( "PARSE", "w" ) );
  FILE *out = fopen( "PARSE", "w" );
  printList( list, out );
  fclose( out );

  // Check it reversed
  // fclose( fopen( "REVERSE", "w" ) );
  // FILE *rev = fopen( "REVERSE", "w" );
  // printListReverse( list, rev );
  // fclose( rev );

  // // Mix them
  // mix( list );
  // printList( list, stdout );
}
