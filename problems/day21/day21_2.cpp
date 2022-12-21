#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <cstdio>

using namespace std;

#define REPORT( X ) cout << #X << " = " << (X) << endl

typedef struct {
  string m1;
  char op;
  string m2;
} Action;

typedef struct {
  string name;
  bool isNumber;
  long n;
  Action act;
} Monkey;

// vector<Monkey> monkeys;
map<string,Monkey> monkeys;

static Monkey parseMonkey( string line )
{
  Monkey m;

  // root: pppw + sjmn
  char nameC[ 4 + 1 ];
  char m1C[ 4 + 1 ];
  char op;
  char m2C[ 4 + 1 ];
  long case1 = sscanf( line.c_str(), "%4s: %s %c %s",
                      nameC, m1C, &op, m2C  );

  if ( case1 == 4 ) {
    string name( nameC );
    string m1( m1C );
    string m2( m2C );
    
    m = {
      .name = name,
      .isNumber = false,
      .act = { m1, op, m2 }
    };
  } else {
    // dbpl: 5
    long n;
    sscanf( line.c_str(), "%4s: %ld", nameC, &n );
    string name( nameC );
    m = {
      .name = name,
      .isNumber = true,
      .n = n
    };
  }

  return m;
}

// static void printMonkeys( FILE *fp )
// {
//   for ( Monkey m : monkeys ) {
//     if ( m.isNumber ) {
//       fprintf( fp, "%s: %ld\n", m.name.c_str(), m.n );
//     } else {
//       fprintf( fp, "%s: %s %c %s\n",
//               m.name.c_str(),
//               m.act.m1.c_str(),
//               m.act.op,
//               m.act.m2.c_str() );
//     }
//   }
// }

static long getVal( string name, int humn )
{
  if ( name == "humn" )
    return humn;

  Monkey m = monkeys[ name ];
  if ( m.isNumber )
    return m.n;
  
  Action act = m.act;
  long n1 = getVal( act.m1, humn );
  long n2 = getVal( act.m2, humn );

  if ( name == "root" ) {
    if ( n1 == n2 ) {
      return humn;
    } else {
      humn++;
      if ( humn % 1000 == 0 ) REPORT( humn );
      return getVal( "root", humn );
    }
  }

  long val;
  switch( act.op ) {
    case '+':
      val = n1 + n2;
      break;
    case '-':
      val = n1 - n2;
      break;
    case '*':
      val = n1 * n2;
      break;
    case '/':
      if ( n1 % n2 != 0 ) {
        humn++;
        if ( humn % 1000 == 0 ) REPORT( humn );
        return getVal( "root", humn );
      }
      val = n1 / n2;
      break;
    default:
      cerr << "getVal failed" << endl;
      exit( 1 );
  }

  if ( val < 0 ) {
    // fprintf( stderr, "%ld %c %ld = %ld\n", n1, act.op, n2, val );
    // cerr << "val overflowed" << endl;
    humn++;
    if ( humn % 1000 == 0 ) REPORT( humn );
    return getVal( "root", humn );
    exit( 1 );
  }

  return val;
}

int main()
{
  string line;
  while ( getline( cin, line ) ) {
    Monkey m = parseMonkey( line );
    // monkeys.push_back( m );
    monkeys[ m.name ] = m;
  }

  // fclose( fopen( "PARSE", "w" ) );
  // FILE *parse = fopen( "PARSE", "w" );
  // printMonkeys( parse );
  // fclose( parse );

  int humn = 1;
  if ( humn % 1000 == 0 ) REPORT( humn );
  cout << getVal( "root", humn ) << endl;
}
