#include <iostream>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

int main()
{
	int ans = 0;
	string line;
	while (getline(cin, line)) {
		int d1 = -1, d2 = -1;
		for (size_t i = 0; i < line.size(); i++) {
			// skip non-digits
			if (!isdigit(line[i])) { continue; }

			int d = line[i] - '0';
			if (d1 == -1) {
				d1 = d;
			} else {
				d2 = d;
			}
		}

		if (d2 == -1) { d2 = d1; };

		ans += 10 * d1 + d2;
	}

	cout << ans << endl;
}
