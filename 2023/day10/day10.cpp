#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define REPORT(X) cout << #X << " = " << (X) << endl

using pi = pair<int, int>;
using Grid = vector<string>;
using Set = vector<vector<bool>>;

#define row first
#define col second

static pi getStart(Grid grid) {
	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid[i].size(); j++) {
			if (grid[i][j] == 'S') {
				return {i, j};
			}
		}
	}

	cerr << "getStart: not found" << endl;
	exit(1);
}

static void addNeighbors(queue<pi>& toVisit,
				const pi& p,
				const Grid& grid,
				const Set& visited)
{
	char type = grid[p.row][p.col];

	// dead tile
	if (type == '.') {
		return;
	}

	pi np = {p.row - 1, p.col};
	pi sp = {p.row + 1, p.col};
	pi ep = {p.row, p.col + 1};
	pi wp = {p.row, p.col - 1};

	// safeguards
	if (visited[np.row][np.col]) {
		np = {0, 0};
	}

	if (visited[sp.row][sp.col]) {
		sp = {0, 0};
	}
	if (visited[ep.row][ep.col]) {
		ep = {0, 0};
	}
	if (visited[wp.row][wp.col]) {
		wp = {0, 0};
	}

	char north = grid[np.row][np.col];
	char south = grid[sp.row][sp.col];

	char east = grid[ep.row][ep.col];
	char west = grid[wp.row][wp.col];

	// special case
	if (type == 'S') {
		if (north == '|' || north == '7' || north == 'F') {
			toVisit.push(np);
		}

		if (south == '|' || south == 'L' || south == 'J') {
			toVisit.push(sp);
		}

		if (east == '-' || east == 'J' || east == '7') {
			toVisit.push(ep);
		}

		if (west == '-' || west == 'L' || west == 'F') {
			toVisit.push(wp);
		}

		return;
	}

	switch(type) {
		case '|':
			toVisit.push(np);
			toVisit.push(sp);
			return;
		case '-':
			toVisit.push(ep);
			toVisit.push(wp);
			return;
		case 'L':
			toVisit.push(np);
			toVisit.push(ep);
			return;
		case 'J':
			toVisit.push(np);
			toVisit.push(wp);
			return;
		case '7':
			toVisit.push(sp);
			toVisit.push(wp);
			return;
		case 'F':
			toVisit.push(sp);
			toVisit.push(ep);
			return;
	}

	REPORT(type);
	exit(1);
}

Grid markedGrid;
static void printGrid()
{
	for (size_t i = 0; i < markedGrid.size(); i++) {
		auto row = markedGrid[i];
		for (size_t j = 0; j < row.size(); j++) {
			char ch = markedGrid[i][j];
			cout << ch;
		}
		cout << endl;
	}
	cout << endl;
}

int main()
{
	Grid grid;
	string line;
	while (getline(cin, line)) {
		if (grid.empty()) {
			string guard(line.size() + 2, '.');
			grid.push_back(guard);
			REPORT(grid.back());
		}

		grid.push_back("." + line + ".");
		REPORT(grid.back());
	}

	// more safeuards
	auto cols = grid[0].size();

	string guard(cols, '.');
	grid.push_back(guard);
	REPORT(grid.back());

	markedGrid = grid;

	auto rows = grid.size();

	pi sp = getStart(grid);


	Set visited(rows);
	for (auto& row : visited) {
		row.resize(cols);
	}

	size_t depth = 0;
	queue<pi> toVisit;
	toVisit.push(sp);

	// bfs
	bool loop = false;
	while (!loop && !toVisit.empty()) {
		int count = toVisit.size();
		while (count--) {
			pi p = toVisit.front();
			toVisit.pop();

			if (visited[p.row][p.col]) {
				if (p == sp) {
					loop = true;
				}
				break;
			} else {
				visited[p.row][p.col] = true;
				markedGrid[p.row][p.col] = '0' + (depth % 10);
			}
			
			addNeighbors(toVisit, p, grid, visited);
		}

		depth++;
	}

	printGrid();

	REPORT(loop);
	REPORT(toVisit.size());

	REPORT((depth-1)/2);
}
