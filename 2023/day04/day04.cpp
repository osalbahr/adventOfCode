#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

static int getPoints(stringstream& ss)
{
	unordered_set<int> winning;
	string winningString;
	getline(ss, winningString, '|');
	stringstream winningStream(winningString);
	int n;
	while (winningStream >> n) {
		winning.insert(n);
	}
		

	int matches = 0;
	while (ss >> n) {
		matches += winning.count(n);
	}
	
	if (matches == 0)
		return 0;
	
	return pow(2, matches - 1);
}

int main()
{
	int total = 0;
	string line;
	while (getline(cin, line)) {
		stringstream ss(line);
		// Card 1:
		string trash;
		ss >> trash >> trash;
		total += getPoints(ss);
	}
	cout << total << endl;
}
