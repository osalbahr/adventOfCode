#include <iostream>
#include <vector>
#include <sstream>

#include <cassert>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl
#define endl "\n"

using ll = long long;
using pll = pair<ll, ll>;

struct Mapping {
	ll dest;
	ll src;
	ll range;
};

static vector<pll> getSeedRanges()
{
	string line;
	getline(cin, line);
	stringstream ss(line);
	
	string trash;
	ss >> trash;

	vector<pll> seedRanges;
	ll start, range;
	while (ss >> start >> range) {
		seedRanges.push_back({start, range});
	}
	
	getline(cin, trash);
	
	return seedRanges;
}

static vector<Mapping> getMap()
{
	vector<Mapping> mapping;

	string line;
	while (getline(cin, line) && line != "") {
		stringstream ss(line);

		ll dest, src, range;
		while (ss >> dest >> src >> range)
			mapping.push_back( {dest, src, range} );
	}

	return mapping;
}

static void printMapping(const Mapping& mapping)
{
	auto src = mapping.src, dest = mapping.dest, range = mapping.range;
	printf("mapping: %lld-%lld -> %lld-%lld\n", src, src + range, dest, dest + range);
}

static void printRanges(const vector<pll>& keyRanges)
{
	cout << "ranges: ";
	for (const auto& range : keyRanges) {
		printf("[%lld, %lld], ", range.first, range.first + range.second);
	}
	cout << endl;
}

static vector<pll> lookUp(vector<pll> keyRanges, vector<Mapping> mp)
{
	vector<pll> results;

	printRanges(keyRanges);

	for (const auto& keyRange : keyRanges) {
		ll minKey = keyRange.first;
		ll maxKey = minKey + keyRange.second;

		for (const auto& mapping : mp) {
			ll minMapping = mapping.src;
			ll maxMapping = mapping.src + mapping.range;

			printMapping(mapping);

			// no overlap
			if (minMapping > maxKey || minKey > maxMapping) {
				continue;
			}

			ll minResult = max(minKey, minMapping);
			ll maxResult = min(maxKey, maxMapping);
			ll range = maxResult - minResult;
	
			ll delta = mapping.dest - mapping.src;
			results.push_back({minResult + delta, range});
			REPORT(results.size());
		}
	}

	printRanges(results);

	cout << endl;
	return results;
}

static ll getMin(ll current, const vector<pll>& ranges)
{
	ll ans = current;
	for (const auto& range : ranges) {
		ans = min(ans, range.first);
	}
	return ans;
}

int main()
{
	vector<pll> seedRanges = getSeedRanges();
	vector<Mapping> mp1 = getMap();
	vector<Mapping> mp2 = getMap();
	vector<Mapping> mp3 = getMap();
	vector<Mapping> mp4 = getMap();
	vector<Mapping> mp5 = getMap();
	vector<Mapping> mp6 = getMap();
	vector<Mapping> mp7 = getMap();

	ll ans = LONG_LONG_MAX;
	for (const auto& seedRange: seedRanges) {
		const auto soil = lookUp({seedRange}, mp1);
		REPORT(soil.size());
		const auto fertilizer = lookUp(soil, mp2);
		const auto water = lookUp(fertilizer, mp3);
		const auto light = lookUp(water, mp4);
		const auto temperature = lookUp(light, mp5);
		const auto humidity = lookUp(temperature, mp6);
		const auto location = lookUp(humidity, mp7);
		ans = getMin(ans, location);
		REPORT(ans);
	}

	REPORT(ans);
}
