#include <iostream>
#include <unordered_map>
#include <vector>

#include <cassert>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

// map node to left/right pair
using MapDS = unordered_map<string, pair<string, string>>;
#define left first
#define right second

static MapDS getMap()
{
	MapDS mp;
	
	string line;
	while (getline(cin, line)) {
		// AAA = (BBB, CCC)
		char srcPtr[4], leftPtr[4], rightPtr[4];
		int matches = sscanf(line.c_str(), "%3s = (%3s, %3s)",
					srcPtr,
					leftPtr,
					rightPtr);
		
		assert(matches == 3);

		string src(srcPtr);
		string left(leftPtr);
		string right(rightPtr);

		mp[src] = {left, right};
	}

	assert(!mp.empty());
	return mp;
}

static string move(const string& pos, MapDS mp, char next)
{
	if (next == 'L') {
		return mp[pos].left;
	} else { // next == 'R'
		return mp[pos].right;
	}
}

int main()
{
	string instructions;
	getline(cin, instructions);

	string trash;
	getline(cin, trash);

	MapDS mp = getMap();

	string pos = "AAA";
	size_t steps;
	for (steps = 0; pos != "ZZZ"; steps++) {
		char next = instructions[ steps % instructions.size() ];
		pos = move(pos, mp, next);
	}

	REPORT(steps);
}