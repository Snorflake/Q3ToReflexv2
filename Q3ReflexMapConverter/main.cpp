#include <iostream>
#include <memory>

#include "parser.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		//insufficient args
		printf("Syntax: Q3ReflexMapConverter.exe [input].map [output].map\n");
		return 1;
	}

	auto mapparser = std::make_unique<parser>();
	mapparser->init(argv[1]);
	if (mapparser->start() == false) {
		printf("Map parsing failed! Is your input correct?\n");
		return 1;
	}

	return 0;
}