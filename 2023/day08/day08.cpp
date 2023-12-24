#include <iostream>
#include <unordered_map>
#include <vector>

#include <cassert>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

// map node to left/right pair
using MapDS = unordered_map<int, pair<int, int>>;
#define left first
#define right second

// 'A' = 0 -> 'Z' = 25
static constexpr int base26ToInt(const char* base26)
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

static int move(const int pos, const MapDS& mp, char next)
{
	if (next == 'L') {
		return mp.at(pos).left;
	} else { // next == 'R'
		return mp.at(pos).right;
	}
}

int main()
{
	string instructions;
	getline(cin, instructions);

	string trash;
	getline(cin, trash);

	MapDS mp = getMap();

	int pos = base26ToInt("AAA");
	size_t steps;
	
	for (steps = 0; pos != base26ToInt("ZZZ"); steps++) {
		char next = instructions[ steps % instructions.size() ];
		pos = move(pos, mp, next);
	}

	REPORT(steps);
}
