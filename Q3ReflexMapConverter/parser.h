#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>

class parser
{
public:
	parser() {}
	/*
	 * initializes all required values
	 */
	void init(std::string mapname);

	/*
	 * starts the parser going
	 */
	bool start();
private:

	std::vector<std::vector<std::string>> lines();

	std::string mapname;
};

#endif