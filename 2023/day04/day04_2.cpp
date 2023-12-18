#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <map>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

unordered_map<int,int> cards;

static void countCards(stringstream& ss, int card)
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
	
	for (int i = 1; i <= matches; i++)
		cards[card + i] += cards[card];
}

int main()
{
	string line;
	while (getline(cin, line)) {
		stringstream ss(line);
		// Card 1:
		string trash;
		int n;
		ss >> trash >> n >> trash;
		cards[n]++;
		countCards(ss, n);
	}
	int total = 0;
	REPORT(cards.size());
	for (const auto [key, val] : cards) {
		total += val;
	}
	cout << total << endl;
}
