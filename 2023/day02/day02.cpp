#include <iostream>
#include <vector>
#include <sstream>      // std::stringstream

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

bool isPossible(stringstream& stream)
{
	int red = 12, green = 13, blue = 14;

	int count;
	string color;
	char last;
	do {
		// special case
		if (! (stream >> count)) {
			return true;
		}

		stream >> color;
		last = color.back();
		if (last == ',' || last == ';') {
			color = color.substr(0, color.size() - 1);
		}

		if ( color == "red" ) {
			red -= count;
		} else if ( color == "green" ) {
			green -= count;
		} else {
			blue -= count;
		}
	} while (last == ',');

	return red >= 0 && green >= 0 && blue >= 0;
}

int main()
{
	int ret = 0;
	string line;
	for (int i = 1; getline(cin, line); i++) {
		stringstream stream(line);
		string trash;
		stream >> trash >> trash;

		bool valid = true;
		while (!stream.eof()) {
			if (!isPossible(stream)) {
				valid = false;
				break;
			}
		}

		if (valid) {
			ret += i;
		}
	}
	cout << ret << endl;
}
