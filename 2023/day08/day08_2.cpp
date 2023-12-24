#include <iostream>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <unordered_set>

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

static unordered_set<string> visited;

static vector<string> move(const vector<string>& pos, MapDS mp, char next)
{

	vector<string> newPos;
	for (const auto& curPos : pos) {
		visited.insert(curPos);
		if (next == 'L') {
			newPos.push_back(mp[curPos].left);
		} else { // next == 'R'
			newPos.push_back(mp[curPos].right);
		}
	}

	// auto newSize = visited.size();

	return newPos;
}

static vector<string> getPos(MapDS mp)
{
	vector<string> pos;
	for (const auto& [key, val] : mp) {
		if (key.back() == 'A') {
			pos.push_back(key);
		}
	}
	return pos;
}

static bool endWithZ(vector<string> pos)
{
	for (const auto& curPos : pos) {
		if (curPos.back() != 'Z') {
			return false;
		}
	}

	return true;
}

int main()
{
	string instructions;
	getline(cin, instructions);

	string trash;
	getline(cin, trash);

	MapDS mp = getMap();

	vector<string> pos = getPos(mp);
	size_t steps;
	for (steps = 0; !endWithZ(pos); steps++) {
		char next = instructions[ steps % instructions.size() ];
		pos = move(pos, mp, next);
		
		if (steps % 10'000 == 0) {
			REPORT(steps);
		}
	}

	REPORT(steps);
}
