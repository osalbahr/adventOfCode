#include <iostream>
#include <vector>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
#define RED "\e[0;31m"
#define RESET "\e[0m"
#define GREEN "\e[0;32m"

static int getNumLength(int num)
{
	int numLength = 1;
	int temp = num;
	while ( (temp /= 10) != 0 ) {
		numLength++;
	}
	return numLength;
}

// return 	0 if not a number
//			-1 if middle of a number
// 			-2 if not a part number
static int getPartNumber(const vector<string>& lines, int i, int j) {
	int num;
	if ( !isdigit(lines[i][j])
		|| sscanf(lines[i].c_str() + j, "%d", &num) != 1 ) {

		return 0;
	}

	// not the beginning
	if (j > 0 && isdigit(lines[i][j - 1])) {
		return -1;
	}
	int numLength = getNumLength(num);
	
	for (int r = i - 1; r <= i + 1; r++) {
		for (int c = j - 1; c <= j + numLength; c++) {
			if (r < 0 || r >= static_cast<int>(lines.size())) {
				continue;
			}
			if (c < 0 || c >= static_cast<int>(lines[i].size())) {
				continue;
			}
			char ch = lines[r][c];

			// bingo
			if (ch != '.' && !isdigit(ch)) {
				return num;
			}
		}
	}

	return -2;
}

int main()
{
	vector<string> lines;
	string line;
	while (getline(cin, line))
		lines.push_back(line);
	
	int total = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		string line = lines[i];
		cout << line << endl;
		for (size_t j = 0; j < lines[i].size(); j++) {
			int num = getPartNumber(lines, i, j);
			if (num > 0) {
				cout << GREEN << num << RESET;
				total += num;
			}

			if (num == -2) {
				sscanf(lines[i].c_str() + j, "%d", &num);
				cout << RED << num << RESET;
			}

			if (!isdigit(lines[i][j])) {
				cout << lines[i][j];
			}
		}
		cout << endl;
	}

	cout << total << endl;
}
