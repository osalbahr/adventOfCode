#include <iostream>
#include <vector>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

vector<string> digitSpelled = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
};

static int getDigit(const string& line, int idx)
{
	if (isdigit(line[idx])) {
		return line[idx] - '0';
	}

	for (size_t i = 0; i < digitSpelled.size(); i++) {
		const auto& spelled = digitSpelled[i];

		// safety check
		if (idx + spelled.size() > line.size()) {
			continue;
		}

		if (line.substr(idx, spelled.size()) == spelled) {
			return i;
		}
	}

	return -1;
}

int main()
{
	int ans = 0;
	string line;
	while (getline(cin, line)) {
		int d1 = -1, d2 = -1;
		for (size_t i = 0; i < line.size(); i++) {
			int d = getDigit(line, i);

			// skip invalid
			if (d == -1) { continue; };

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
