#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using ll = long long;

#define REPORT(X) cout << #X << " = " << (X) << endl

static vector<ll> getNums()
{
	string line;
	getline(cin, line);
	stringstream ss(line);

	string trash;
	ss >> trash;

	string mash;
	ll n;
	while (ss >> n) {
		mash += to_string(n);
	}

	stringstream ss2(mash);
	ss2 >> n;

	REPORT(n);
	return {n};
}

static ll getWays(ll time, ll dist)
{
	ll ways = 0;
	for (ll t = 0; t <= time; t++) {
		if ( t * (time - t) > dist ) {
			ways++;
		}
	}
	return ways;
}

int main()
{
	vector<ll> times = getNums();
	REPORT(times.size());
	vector<ll> dists = getNums();
	REPORT(dists.size());

	ll ans = 1;
	for (size_t i = 0; i < times.size(); i++) {
		ll ways = getWays(times[i], dists[i]);
		ans *= ways;
	}

	REPORT(ans);
}
