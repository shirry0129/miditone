#include "ScoreReader.hpp"
#include <iostream>

int main() {

	score::ScoreReader reader("test/000_0905.txt");

	score::Header header;
	reader.readHeader(header);

	using std::cout;
	cout << "title	:" << header.title << '\n';
	cout << "artist	:" << header.artist << '\n';
	cout << "genre	:" << header.genre << '\n';
	cout << "level	:";
	for (const auto &l : header.level)
		cout << l << "/";
	cout << '\n';

	for (const auto &t : header.tempo) {
		cout << "tempo	:"
			<< "bar " << t.bar
			<< " and "
			<< t.posInBar.get_str() << " bar offset"
			<< "   ♪=" << t.tempo << '\n';
	}

	for (const auto &b : header.beat) {
		cout << "beat	:"
			<< "bar " << b.bar
			<< "   " << b.beat.get_str() << '\n';
	}

	return 0;
}