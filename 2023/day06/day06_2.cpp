#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

static vector<int> getNums()
{
	string line;
	getline(cin, line);
	stringstream ss(line);

	string trash;
	ss >> trash;

	vector<int> nums;
	int n;
	while (ss >> n) {
		nums.push_back(n);
	}

	return nums;
}

static int getWays(int time, int dist)
{
	int ways = 0;
	for (int t = 0; t <= time; t++) {
		if ( t * (time - t) > dist ) {
			ways++;
		}
	}
	return ways;
}

int main()
{
	vector<int> times = getNums();
	vector<int> dists = getNums();

	int ans = 1;
	for (size_t i = 0; i < times.size(); i++) {
		int ways = getWays(times[i], dists[i]);
		ans *= ways;
	}

	REPORT(ans);
}
