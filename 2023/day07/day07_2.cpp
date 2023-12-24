#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>

#include <cmath>

using namespace std;

using ll = long long;

#define REPORT(X) cout << #X << " = " << (X) << endl

#define endl '\n'

static int getType(const string& hand)
{
	enum {
		highCard,
		onePair,
		twoPair,
		threeKind,
		fullHouse,
		fourKind,
		fiveKind,
	};

	unordered_map<char, int> charToCount;
	for (const auto ch : hand) {
		charToCount[ch]++;
	}

	int maxCount = 1;
	int pairCount = 0;
	for (const auto [ch, count] : charToCount) {
		maxCount = max(maxCount, count);
		if (count == 2) pairCount++;
	}

	switch(maxCount) {
		case 5:
			return fiveKind;
		case 4:
			return fourKind;
		case 3:
			if (pairCount == 1) { return fullHouse; }
			return threeKind;
	}

	switch(pairCount) {
		case 2:
			return twoPair;
		case 1:
			return onePair;
	}

	// all distinct
	return highCard;
}

static string transform(const string& hand, const map<char,int>& char2strength)
{
	string transformed = hand;
	for (size_t i = 0; i < transformed.size(); i++) {
		char ch = transformed[i];
		transformed[i] = char2strength.at(ch);
	}
	return transformed;
}

static vector<string> jokerize(const string& hand, const string& order, int startIdx)
{
	vector<int> indices;
	for (size_t i = startIdx; i < hand.size(); i++) {
		if (hand[i] == 'J') {
			indices.push_back(i);
		}
	}

	if (indices.size() == 0) {
		return {hand};
	}

	vector<string> hands;

	for (const auto i : indices) {
		string newHand = hand;
		for (const auto ch : order) {
			newHand[i] = ch;

			vector<string> newHands = jokerize(newHand, order, i + 1);
			hands.insert(hands.end(), newHands.begin(), newHands.end());
		}
	}

	return hands;
}

int main()
{
	map<char,int> char2strength;
	string order = "AKQJT9876543210";
	reverse(order.begin(), order.end());
	for (size_t i = 0; i < order.size(); i++) {
		char2strength[order[i]] = i;
	}

	vector<string> hands;
	unordered_map<string, int> handToBid;

	string hand;
	int bid;
	while (cin >> hand >> bid) {
		hands.push_back(hand);
		handToBid[hand] = bid;
	}

	// (type, transformed, hand)
	vector<tuple<int, string, string>> toSort;
	for (const auto& hand : hands) {
		REPORT(hand);
		vector<string> jokerized = jokerize(hand, order, 0);
		REPORT(jokerized.size());

		int type = 0;
		for (const auto& newHand : jokerized) {
			// REPORT(newHand);
			type = max(type, getType(newHand));
		}

		REPORT(type);

		string transformed = transform(hand, char2strength);
		toSort.push_back( {type, transformed, hand} );
	}

	sort(toSort.begin(), toSort.end());

	int ans = 0;
	int rank = 1;
	for (const auto& [type, transformed, hand] : toSort) {
		int bid = handToBid[hand];
		ans += (rank++) * bid;
	}

	REPORT(ans);
}
