#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

/*
 * 	Struct to store the current state.
 *
 * 		path: String of movements taken by the agent.
 * 		pos: (x,y) coordinates with the position of the agent.
 * 		crates: [(x,y)] array of coordinates of each crate in the map.
 * 		goalCrates: Int representing the number of current creates in
 * 			a goal position.
 */

class St {
  public:
	std::string path;
	std::pair<uint8_t, uint8_t> pos;
	std::vector<std::pair<uint8_t, uint8_t>> crates;
	uint8_t goalCrates;

	bool operator<(const St &other) const {
		std::vector<std::pair<uint8_t, uint8_t>> set1, set2;
		set1 = crates;
		set1.push_back(pos);
		set2 = other.crates;
		set2.push_back(other.pos);
		return set1 < set2;
	}
};

/*
 * 	Function to read the map line by line and store it in a 2D matrix. Remove
 * 		the crates and the agent of the map. Said information is stored in the
 * 		state struct. The resulting map is just the borders and end pouint8_ts
 *  	for the crates.
 *
 * 		state: initially empty state of the agent.
 */

std::vector<std::string> readMap(St &state) {
	std::string tempString;
	std::vector<std::string> map;
	uint8_t j = 0;

	while (std::getline(std::cin, tempString)) {
		for (unsigned long i = 0; i < tempString.length(); ++i) {
			switch (tempString.at(i)) {
			case '@':
				state.pos.first = i;
				state.pos.second = j;
				tempString[i] = ' ';
				break;
			case '$':
				state.crates.push_back(std::make_pair(i, j));
				tempString[i] = ' ';
				break;
			default:
				break;
			}
		}
		map.push_back(tempString);
		j++;
	}

	return map;
}

/*
 * 	Function to validate that the new state of the map is a valid state.
 * 		Return false if either the box is moved to an invalid position or
 * 		is moved to the current position of another box. Also return false
 * 		if the box is trapped in a corner that has no goal state in it.
 * 		Return true if the box is in a goal position or free to move any more.
 */

bool checkCrateCorners(const std::vector<std::string> &map, std::pair<uint8_t, uint8_t> pos) {
	if (map[pos.second][pos.first] == '.') {
		return true;
	}
	if ((map[pos.second][pos.first - 1] == '#' && // Check left and down
		 map[pos.second + 1][pos.first] == '#') ||
		(map[pos.second][pos.first - 1] == '#' && // Check left and up
		 map[pos.second - 1][pos.first] == '#') ||
		(map[pos.second][pos.first + 1] == '#' && // Check right and down
		 map[pos.second + 1][pos.first] == '#') ||
		(map[pos.second][pos.first + 1] == '#' && // Check right and up
		 map[pos.second - 1][pos.first] == '#')) {
		return false;
	}
	return true;
}

/*
 * 	Function to validate the new state after moving a box. If the new position
 * 		for the box is either a corner, conflicts with another box or goes over
 * 		a wall, return false to signal an invalid state.
 *
 * 		it: vector iterator to the coordinates of a box.
 */

bool validateSt(St &st, const std::vector<std::string> &map, std::set<St> &setOfSts) {
	auto it = std::find(st.crates.begin(), st.crates.end(), st.pos);
	if (it != st.crates.end()) {
		auto newPos = *it;
		switch (st.path.back()) {
		case 'L':
			newPos.first--;
			break;
		case 'R':
			newPos.first++;
			break;
		case 'U':
			newPos.second--;
			break;
		case 'D':
			newPos.second++;
			break;
		default:
			break;
		}
		if (map[newPos.second][newPos.first] == '#' ||
			std::find(st.crates.begin(), st.crates.end(), newPos) != st.crates.end() ||
			!checkCrateCorners(map, newPos)) {
			return false;
		}
		*it = newPos;
		if (map[newPos.second][newPos.first] == '.') {
			st.goalCrates++;
		}
		if (map[st.pos.second][st.pos.first] == '.') {
			st.goalCrates--;
		}
	}
	if (setOfSts.find(st) != setOfSts.end()) {
		return false;
	}

	setOfSts.insert(st);
	return true;
}

std::string findPath(const std::vector<std::string> &map, St initialSt) {
	const char noAccess = '#';
	uint8_t rows = map.size(), columns;
	std::queue<St> stQ;
	std::set<St> setOfSts;

	stQ.push(initialSt);
	while (!stQ.empty()) {
		St stTemp, st;
		stTemp = st = stQ.front();
		columns = map[st.pos.second].size();

		if (st.goalCrates == st.crates.size()) {
			return st.path;
		}

		if (st.pos.first > 0 && map[st.pos.second][st.pos.first - 1] != noAccess) {
			stTemp.pos.first--;
			stTemp.path += "L";
			if (validateSt(stTemp, map, setOfSts)) {
				stQ.push(stTemp);
			}
			stTemp = st;
		}
		if (st.pos.second > 0 && map[st.pos.second - 1][st.pos.first] != noAccess) {
			stTemp.pos.second--;
			stTemp.path += "U";
			if (validateSt(stTemp, map, setOfSts)) {
				stQ.push(stTemp);
			}
			stTemp = st;
		}
		if (st.pos.first < columns && map[st.pos.second][st.pos.first + 1] != noAccess) {
			stTemp.pos.first++;
			stTemp.path += "R";
			if (validateSt(stTemp, map, setOfSts)) {
				stQ.push(stTemp);
			}
			stTemp = st;
		}
		if (st.pos.second < rows && map[st.pos.second + 1][st.pos.first] != noAccess) {
			stTemp.pos.second++;
			stTemp.path += "D";
			if (validateSt(stTemp, map, setOfSts)) {
				stQ.push(stTemp);
			}
			stTemp = st;
		}

		stQ.pop();
	}

	return "";
}

int main() {
	std::string ans;
	St initialSt = {"", std::make_pair(0, 0), {}, 0};
	std::vector<std::string> map = readMap(initialSt);

	ans = findPath(map, initialSt);

	std::cout << ans << '\n';

	return 0;
}
