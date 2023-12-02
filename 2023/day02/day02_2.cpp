#include <iostream>
#include <vector>
#include <sstream>      // std::stringstream

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

int redGlobal = 0, greenGlobal = 0, blueGlobal = 0;

void simulate(stringstream& stream)
{
	int red = 0, green = 0, blue = 0;

	int count;
	string color;
	char last;
	do {
		// special case
		if (! (stream >> count)) {
			return;
		}

		stream >> color;
		last = color.back();
		if (last == ',' || last == ';') {
			color = color.substr(0, color.size() - 1);
		}

		if ( color == "red" ) {
			red += count;
		} else if ( color == "green" ) {
			green += count;
		} else {
			blue += count;
		}
	} while (last == ',');

	redGlobal = max(redGlobal, red);
	greenGlobal = max(greenGlobal, green);
	blueGlobal = max(blueGlobal, blue);
}

int main()
{
	int totalPower = 0;

	string line;
	while (getline(cin, line)) {
		stringstream stream(line);
		string trash;
		stream >> trash >> trash;

		while (!stream.eof()) {
			simulate(stream);
		}

		int power = redGlobal * greenGlobal * blueGlobal;
		totalPower += power;
		redGlobal = greenGlobal = blueGlobal = 0;
	}
	cout << totalPower << endl;
}
