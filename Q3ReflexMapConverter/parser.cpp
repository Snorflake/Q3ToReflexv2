#include "parser.h"
#include <fstream>
#include <cstdlib>


void parser::init(const std::string& mapname)
{
	this->mapname = mapname;

}

bool parser::start()
{
	auto all_lines = lines();
	if (all_lines.empty()) return false;


	return true;
}

//loosely based off Q3ToReflex parser
std::vector<std::vector<std::string>> parser::lines()
{
	std::ifstream file;
	std::vector<std::vector<std::string>> ret;

	file.open(mapname.c_str(), std::ios::in);
	if (file.is_open()) {
		std::string line;

		//get all those mother fucking lines
		while (std::getline(file, line)) { //this returns false if there are no lines left

			//create token array
			ret.emplace_back(); //basically push_back new array except proper because c++ is cool

			char szline[2048]; //HARDCODED VALUES WEWEWEWEWEWE
			const char* delim = " \t\n\r";
			char* token;
			char* context;

			//put that fuckin string into that char array
			strcpy_s(szline, line.c_str());
			token = strtok_s(szline, delim, &context); // split that shit from the delimiter
			while (token != nullptr) {
				//while theres still shit left on the line
				
				//fuck this loop off if its a comment
				if (strcmp("//", token) == 0) {
					break; //next line bitch nothing to see here
				}
				else {
					ret.back().push_back(token); //put the token in the array so we can rape it later
					token = strtok_s(nullptr, delim, &context); //no need to pass a sourcce string it goes off context
				}
			}
		}
		//we're out of lines so get rid of this file handle
		file.close();
	}
	//if it didnt open itll return a blank array. just check for that and you're good to go

	return ret;
}