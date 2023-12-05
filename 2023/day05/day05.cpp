#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

struct Mapping {
	long long dest;
	long long src;
	long long range;
};

static vector<long long> getSeeds()
{
	string line;
	getline(cin, line);
	stringstream ss(line);
	
	string trash;
	ss >> trash;

	vector<long long> seeds;
	long long s;
	while (ss >> s)
		seeds.push_back(s);
	
	getline(cin, trash);
	
	return seeds;
}

static vector<Mapping> getMap()
{
	vector<Mapping> mapping;

	string line;
	while (getline(cin, line) && line != "") {
		stringstream ss(line);

		long long dest, src, range;
		while (ss >> dest >> src >> range)
			mapping.push_back( {dest, src, range} );
	}

	return mapping;
}

static long long lookUp(long long key, vector<Mapping> mp)
{
	for (const auto& mapping : mp) {
		if (key > mapping.src && key <= mapping.src + mapping.range) {
			return mapping.dest + ( key - mapping.src );
		}
	}

	return key;
}

int main()
{
	vector<long long> seeds = getSeeds();
	vector<Mapping> mp1 = getMap();
	vector<Mapping> mp2 = getMap();
	vector<Mapping> mp3 = getMap();
	vector<Mapping> mp4 = getMap();
	vector<Mapping> mp5 = getMap();
	vector<Mapping> mp6 = getMap();
	vector<Mapping> mp7 = getMap();

	long long ans = LONG_LONG_MAX;
	for (const auto seed : seeds) {
		REPORT(seed);
		long long v1 = lookUp(seed, mp1);
		long long v2 = lookUp(v1, mp2);
		long long v3 = lookUp(v2, mp3);
		long long v4 = lookUp(v3, mp4);
		long long v5 = lookUp(v4, mp5);
		long long v6 = lookUp(v5, mp6);
		long long location = lookUp(v6, mp7);
		REPORT(location);

		ans = min(ans, location);
	}

	REPORT(ans);
}
