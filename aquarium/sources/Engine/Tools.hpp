#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

#include <string>
#include <istream>
#include <fstream>      
#include <sstream>
#include <Engine/Global.hpp>

class Tools 
{
public:
	static std::string GetFileContent(std::string filepath) {
		if(global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());
		std::ifstream file(filepath);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string res = buffer.str();
			file.close();
			return res;
		}
		else {
			return "";
		}
	}
};

#endif // !__TOOLS_HPP__
