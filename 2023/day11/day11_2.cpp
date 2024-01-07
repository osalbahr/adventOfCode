#include <iostream>
#include <vector>

#include <cassert>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

struct Galaxy {
	int row;
	int col;
	// auto operator<=>(const Galaxy&) const = default;
};

static int getDist(const Galaxy& ga,
	const Galaxy& gb,
	const vector<bool>& rowIsEmpty,
	const vector<bool>& colIsEmpty)
{
	int dr = abs(ga.row - gb.row);
	int dc = abs(ga.col - gb.col);

	int dist = dr + dc;
	for (int i = min(ga.row, gb.row) + 1; i < max(gb.row, gb.row); i++) {
		dist += (1'000'000 - 1) * rowIsEmpty[i];
	}
	for (int j = min(ga.col, gb.col) + 1; j < max(ga.col, gb.col); j++) {
		dist += (1'000'000 - 1) * colIsEmpty[j];
	}
	return dist;
}

int main()
{
	vector<string> grid;

	string line;
	while (getline(cin, line)) {
		grid.push_back(line);
	}

	vector<bool> rowIsEmpty(grid.size(), true);
	vector<bool> colIsEmpty(grid[0].size(), true);

	vector<Galaxy> galaxies;

	// find galaxies
	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid[0].size(); j++) {
			if (grid[i][j] == '#') {
				rowIsEmpty[i] = false;
				colIsEmpty[j] = false;

				Galaxy galaxy = {static_cast<int>(i), static_cast<int>(j)};
				galaxies.push_back(galaxy);
			}
		}
	}

	size_t ans = 0;
	for (size_t aIdx = 0; aIdx < galaxies.size(); aIdx++) {
		for (size_t bIdx = aIdx + 1; bIdx < galaxies.size(); bIdx++) {
			const auto& ga = galaxies[aIdx];
			const auto& gb = galaxies[bIdx];

			int dist = getDist(ga, gb, rowIsEmpty, colIsEmpty);	

			ans += dist;
		}
	}

	REPORT(ans);
}
