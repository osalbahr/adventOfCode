#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <cstdio>

using namespace std;

#define REPORT( X )// cout << #X << " = " << ( X ) << endl

vector<string> tokenize( string s )
{
  vector<string> tokens;
  int idx = 1; // skip leading [
  string token = "";
  while( idx < s.size() ) {
    if ( idx == s.size() - 1 ) {
      if ( !token.empty() ) {
        tokens.push_back( token );
        //REPORT( token );
        token = "";
      }
      break;
    }
    if ( s[ idx ] == '[' ) {
      // cout << "// gobble it up" << endl;
      token += '[';
      // tokens.push_back( token );
      // //REPORT( token );

      int opening = 1;
      idx++;
      while( true ) {
        switch( s[ idx ] ) {
          case '[':
          opening++;
          break;
          case ']':
          opening--;
          break;
        }
        token += s[ idx ];
        if ( opening == 0 )
          break;
        idx++;
      }
      // cout << "gobbled" << endl;
      REPORT( token );
      tokens.push_back( token );
      //REPORT( token );
      token = "";
      idx++;
      continue;
    }
    if ( s[ idx ] == ',' ) {
      if ( !token.empty() ) {
        tokens.push_back( token );
        token = "";
      }
      idx++;
      continue;
    }

    token += s[ idx ];
    idx++;
  }

  // cout << "tokenized" << endl;
  REPORT( s );
  for ( auto token : tokens )
    REPORT( token );
  return tokens;
}

// -1 or 0 or 1
int matchVec( vector<string> v1, vector<string> v2 )
{
  // v1.size() == 0 ? cout << "v1 empty" << endl
  //                 : REPORT( v1[ 0 ] );
  // v2.size() == 0 ? cout << "v2 empty" << endl
  //                 : REPORT( v2[ 0 ] );
  for ( int i = 0; i < v1.size(); i++ ) {
    if ( i >= v2.size() )
      return 1;
    
    string s1 = v1[ i ];
    string s2 = v2[ i ];
    REPORT( s1 );
    REPORT( s2 );

    stringstream ss1( s1 );
    stringstream ss2( s2 );

    int n1, n2;
    if ( ss1 >> n1 && ss2 >> n2 ) {
      if ( n1 < n2 )
        return -1;
      if ( n1 > n2 )
        return 1;
    } else {
      // Promote them
      if ( s1[ 0 ] != '[' ) {
        s1 = '[' + s1 + ']';
      }
      if ( s2[ 0 ] != '[' )
        s2 = '[' + s2 + ']';
      // cout << "Recursive{" << endl;
      // REPORT( s1 );
      // REPORT( s2 );
      // cout << "}Recursive" << endl;
      int val = matchVec( tokenize( s1 ), tokenize( s2 ) );
      if ( val != 0 )
        return val;
    }
  }

  // Did left run out or are they equal
  // cout << "RUNNING" << endl;
  return v1.size() < v2.size() ? -1 : 0;
}

int match( string s1, string s2 )
{
  // Promote them
  if ( s1[ 0 ] != '[' ) {
    s1 = '[' + s1 + ']';
  }
  if ( s2[ 0 ] != '[' )
    s2 = '[' + s2 + ']';

  int val = matchVec( tokenize( s1 ), tokenize( s2 ) );
  // printf( "matchVec( tokenize( %s ), tokenize( %s ) ); = %d\n", s1.c_str(), s2.c_str(), val );
  return val;
}

int main()
{
  vector<string> packets;
  string line1, line2;
  
  while ( getline( cin, line1 ) && getline( cin, line2 ) ) {
    // cout << line1 << endl;
    // cout << line2 << endl;
    packets.push_back( line1 );
    packets.push_back( line2 );

    getline( cin, line1 );
  }

  // https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects
  struct less_than_key
  {
    inline bool operator() (const string& s1, const string& s2)
    {
        return match( s1, s2 ) == -1;
    }
  };

  packets.push_back( "[[2]]" );
  packets.push_back( "[[6]]" );
  sort( packets.begin(), packets.end(), less_than_key() );
  cout << "Sorted" << endl;
  // for ( string packet : packets )
  //   cout << packet << endl;

  int multiply = 1;
  for ( int i = 1; i <= packets.size(); i++ ) {
    int idx = i - 1;
    if ( packets[ idx ] == "[[2]]" ||  packets[ idx ] == "[[6]]" ) {
      cout << i << endl;
      REPORT( idx );
      REPORT( packets[ idx ] );
      multiply *= i;
      REPORT( multiply );
    }
  }

  cout << multiply << endl;

  cout << "naive" << endl;
  vector<string> sortedVec;
  set<string> sortedSet;
  for ( int i = 0; i < packets.size(); i++ ) {
    for ( auto a : packets ) {
      if ( sortedSet.find( a ) != sortedSet.end() )
        continue;
      string min = a;
      for ( auto b : packets ) {
        if ( sortedSet.find( b ) != sortedSet.end() )
          continue;
        if ( match( b, min ) < 0 )
          min = b;
      }
      // cout << min << endl;
      sortedSet.insert( min );
      sortedVec.push_back( min );
    }
  }

  multiply = 1;
  for ( int i = 1; i <= sortedVec.size(); i++ ) {
    int idx = i - 1;
    if ( sortedVec[ idx ] == "[[2]]" ||  sortedVec[ idx ] == "[[6]]" ) {
      cout << i << endl;
      REPORT( idx );
      REPORT( sortedVec[ idx ] );
      multiply *= i;
      REPORT( multiply );
    }
  }

  cout << multiply << endl;
}
