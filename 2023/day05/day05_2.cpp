#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl
#define endl "\n"

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
	long long start, range;
	while (ss >> start >> range) {
		for (long long i = start; i <= start + range; i++) {
			seeds.push_back(i);
			if (seeds.size() % 10'000'000 == 0)
				REPORT(seeds.size());
		}
		// seeds.push_back(start);
		// seeds.push_back(range);
	}
	
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
		if (key >= mapping.src && key <= mapping.src + mapping.range) {
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
		// REPORT(seed);
	// for (size_t i = 0; i < seeds.size() - 1; i++) {
	// 	for (long long seed = seeds[i]; seed <= seeds[i] + seeds[i+1]; i++) {
		long long soil = lookUp(seed, mp1);
		long long fertilizer = lookUp(soil, mp2);
		long long water = lookUp(fertilizer, mp3);
		long long light = lookUp(water, mp4);
		long long temperature = lookUp(light, mp5);
		long long humidity = lookUp(temperature, mp6);
		long long location = lookUp(humidity, mp7);
		ans = min(ans, location);
		
		// REPORT(soil);
		// REPORT(fertilizer);
		// REPORT(water);
		// REPORT(light);
		// REPORT(temperature);
		// REPORT(humidity);
		// REPORT(location);
		// REPORT("");
		// }
	}

	REPORT(ans);
}
