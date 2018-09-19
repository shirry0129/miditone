#include "../ScoreReader.hpp"

int WinMain() {
	
	score::ScoreReader reader("../test/438_0905.txt");

	score::Header header;
	reader.readHeader(header, "header");

	using std::cout;

	cout << U"title" << header.title.c_str() << '\n';

	return 0;
}