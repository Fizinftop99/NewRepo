#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

void remove_comments(std::string file) {
	enum class State {
		LOUNCH,
		WITHIN_SINGULAR,
		WITHIN_PLURAL,
		WITHIN_STRING,
		BEYOND,
		NOT_FINISHING_STRING,
		FINISHING_MULTI
	} state = State::BEYOND;
	std::vector<char> contents;
	std::ifstream reader(file, std::ios::binary);
	char c;
	while (reader.get(c)) {
		contents.push_back(c);
	}
	std::vector<char> results;
	size_t pos = 0;
	for (auto c : contents) {
		switch (state) {
		case State::BEYOND:
			if (c == '/') {
				state = State::LOUNCH;
			}
			else if (c == '"') {
				state = State::WITHIN_STRING;
			}
			results.push_back(c);
			break;
		case State::LOUNCH:
			if (c == '/') {
				state = State::WITHIN_SINGULAR;
				results.erase(std::prev(results.end(), 1));
			}
			else if (c == '*') {
				state = State::WITHIN_PLURAL;
				results.erase(std::prev(results.end(), 1));
			}
			else {
				state = State::BEYOND;
				results.push_back(c);
			}
			break;
		case State::WITHIN_PLURAL:
			if (c == '*') {
				state = State::FINISHING_MULTI;
			}
			break;
		case State::WITHIN_SINGULAR:
			if (c == '\r') {
				state = State::BEYOND;
				results.push_back(c);
			}
			break;
		case State::WITHIN_STRING:
			if (c == '\\') {
				state = State::NOT_FINISHING_STRING;
			}
			else if (c == '"') {
				state = State::BEYOND;
			}
			results.push_back(c);
			break;
		case State::NOT_FINISHING_STRING:
			if (c == '"') {
				state = State::WITHIN_STRING;
			}
			results.push_back(c);
			break;
		case State::FINISHING_MULTI:
			if (c == '/') {
				state = State::BEYOND;
			}
			else {
				state = State::WITHIN_PLURAL;
			}
			break;
		}
	}
	std::ofstream writer(file, std::ios::binary);
	for (auto c : results) {
		writer << c;
	}
}

int main() {
	remove_comments("input.txt");
	return 0;
}