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
		if (global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());
		std::ifstream file(filepath);

		if (!file.is_open()) return ""; //if openning failed, return empty

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string res = buffer.str();
		file.close();
		return res;
	}

	static std::vector<unsigned char> GetFileContentByte(std::string filepath) {
		if (global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());

		std::ifstream file(filepath, std::ifstream::binary);

		if (!file.is_open()) return std::vector<unsigned char>(); //if openning failed, return empty

		return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	}

	static std::vector<std::string> SplitString(std::string str, std::string splitter) {
		std::vector<std::string> res;
		size_t lastPos = 0;
		while (lastPos >= 0 && lastPos < str.length()) {
			size_t pos = str.find_first_of(splitter, lastPos);
			if (pos >= 0) {
				std::string tmp = str.substr(lastPos, pos - lastPos);
				if (tmp.size() > 0) {
					res.push_back(tmp);
				}
				lastPos = pos < str.length() ? pos + 1 : -1;
			}
			else {
				lastPos = -1;
			}
		}
		if (res.size() == 0) {
			res.push_back(str);
		}
		return res;
	}

	static void ObjLoader(std::string filepath, std::vector<glm::vec3>* pts, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uvs, std::vector<std::vector<int>>* faces) {
		if (global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());
		std::ifstream file(filepath);
		if (!file.is_open()) return; //if openning failed, return

		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> tmp = SplitString(line, " ");
			if (tmp[0].find_first_of("#") == -1) {
				if (tmp[0].compare("v") == 0) {
					pts->push_back(glm::vec3(std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3])));
				}
				else if (tmp[0].compare("vt") == 0) {
					uvs->push_back(glm::vec2(std::stod(tmp[1]), std::stod(tmp[2])));
				}
				else if (tmp[0].compare("vn") == 0) {
					normals->push_back(glm::vec3(std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3])));
				}
				else if (tmp[0].compare("f") == 0) {
					std::vector<int> ids;
					for (int i = 1; i < tmp.size(); i++) {
						if (tmp[i].find_first_of("#") == -1) {
							std::vector<std::string> fTmp = SplitString(tmp[i], "/");
							if (fTmp.size() >= 1) {
								ids.push_back(std::stoi(fTmp[0])-1);
							}
						}
						else {
							i = tmp.size();
						}
					}
					if (ids.size() >= 3) {
						faces->push_back(ids);
					}
				}
			}
		}

	}

	static std::string GetExtensionLower(std::string filename) {
		std::string ext = filename.substr(filename.find_last_of(".") + 1);
		std::string res = "";
		for (int i = 0, max = ext.size(); i < max; i++) {
			res += tolower(ext.at(i));
		}
		return res;
	}
};

#endif // !__TOOLS_HPP__
