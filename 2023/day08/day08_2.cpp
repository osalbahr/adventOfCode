#include <iostream>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <unordered_set>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

// map node to left/right pair
using MapDS = unordered_map<int, pair<int, int>>;
#define left first
#define right second

// 'A' = 0 -> 'Z' = 25
static int base26ToInt(const char* base26)
{
	int ret = 0;
	for (int i = 2, exp = 0; i >= 0; i--, exp++) {
		ret += (base26[i] - 'A') * pow(26, exp);
	}
	return ret;
}

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

		int src = base26ToInt(srcPtr);
		int left = base26ToInt(leftPtr);
		int right = base26ToInt(rightPtr);

		mp[src] = {left, right};
	}

	assert(!mp.empty());
	return mp;
}

static vector<int> move(const vector<int>& pos, const MapDS& mp, char next)
{

	vector<int> newPos;
	for (const auto& curPos : pos) {
		if (next == 'L') {
			newPos.push_back(mp.at(curPos).left);
		} else { // next == 'R'
			newPos.push_back(mp.at(curPos).right);
		}
	}

	// auto newSize = visited.size();

	return newPos;
}

static vector<int> getPos(MapDS mp)
{
	vector<int> pos;
	for (const auto& [key, val] : mp) {
		if (key % 26  == 0) { // ends with 'A'
			pos.push_back(key);
		}
	}
	return pos;
}

static bool endWithZ(vector<int> pos)
{
	for (const auto& curPos : pos) {
		if (curPos % 26 != 25) { // end with 'Z'
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

	vector<int> pos = getPos(mp);
	size_t steps;
	for (steps = 0; !endWithZ(pos); steps++) {
		char next = instructions[ steps % instructions.size() ];
		pos = move(pos, mp, next);
		
		if (steps % 1'000'000 == 0) {
			REPORT(steps);
		}
	}

	REPORT(steps);
}
