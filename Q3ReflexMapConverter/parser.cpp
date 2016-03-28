#include "parser.h"
#include <fstream>
#include <cstdlib>
#include "entity.h"

void parser::init(const std::string& mapname)
{
	this->mapname = mapname;

}

bool parser::start()
{
	auto&& all_lines = lines();
	if (all_lines.empty()) return false;

	parsebrushes(all_lines);


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
				if (std::string(token).find("//") != std::string::npos) {
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
enum class parsestate
{
	start,
	entity,
	brush,
	patch
	//TODO: others
};
void parser::parsebrushes(const std::vector<std::vector<std::string>>& line_array)
{
	parsestate state = parsestate::start;
	parsestate prevstate = state;

	entity<WorldSpawn> worldspawn;
	entity_base::brush* brush = 0;
	entity_base::brush::face* face = 0;
	for (auto&& it : line_array) {
		if (it.empty()) continue;
		if (state == parsestate::start) {
			if (it[0].find("classname") != std::string::npos && it[1].find("worldspawn") != std::string::npos) {
				state = parsestate::entity;
				prevstate = parsestate::start;
			}
			//TODO: other types
		}
		else if (state == parsestate::entity) {
			if (it[0].find("{") != std::string::npos) {
				state = parsestate::brush;
				prevstate = parsestate::entity;
				brush = &worldspawn.new_brush();
			}
			if (it[0].find("}") != std::string::npos) {
				state = prevstate;
				prevstate = parsestate::entity;
			}
		}
		else if (state == parsestate::brush) {
			if (it[0].find("(") == std::string::npos) {
				//TODO: handle this. this isn't a brush
			}
			else {

			}
			if (it[0].find("}") != std::string::npos) {

				state = prevstate;
				prevstate = parsestate::brush;
				//ERROR: Some reference error or shit, something is fucking up and it's not populating worldspawn's face.
				continue;
			}
			if (it[0].find("patchDef") != std::string::npos) {
				//this is NOT a brush!
				state = parsestate::patch;
				continue;
			}
#define TF(i) std::stof(it[i]) //TF == to float
			auto&& face = brush->new_face(); //TODO: material names
			face.add_vertex(TF(1), TF(2), TF(3));
			face.add_vertex(TF(6), TF(7), TF(8));
			face.add_vertex(TF(11), TF(12), TF(13));
		}
	}
	worldspawn.print(stdout);
}