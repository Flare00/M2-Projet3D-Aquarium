#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

#include <string>
#include <istream>
#include <fstream>      
#include <sstream>
#include <Engine/Global.hpp>
/// <summary>
/// A tools class for different operations.
/// </summary>
class Tools
{
public:
	/// <summary>
	/// Return the content of a file.
	/// </summary>
	/// <param name="filepath">The path of the file.</param>
	/// <returns>A long string corresponding to the file content.</returns>
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

	/// <summary>
	/// Return the file content in bytes.
	/// </summary>
	/// <param name="filepath">the path of the file.</param>
	/// <returns>The file content in a byte vector.</returns>
	static std::vector<unsigned char> GetFileContentByte(std::string filepath) {
		if (global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());

		std::ifstream file(filepath, std::ifstream::binary);

		if (!file.is_open()) return std::vector<unsigned char>(); //if openning failed, return empty

		return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	}


	/// <summary>
	/// Split a string.
	/// </summary>
	/// <param name="str">The string to split</param>
	/// <param name="splitter">The element that split the string</param>
	/// <returns>The splitted string.</returns>
	static std::vector<std::string> SplitString(std::string str, std::string splitter) {
		std::vector<std::string> res;
		size_t lastPos = 0;
		while (lastPos >= 0 && lastPos < str.length()) {
			size_t pos = str.find_first_of(splitter, lastPos);
			if (pos >= 0) {
				std::string tmp = str.substr(lastPos, pos - lastPos);
				res.push_back(tmp);
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


	/// <summary>
	/// Simple data for ObjLoader
	/// </summary>
	struct PointFaceData{
		int vertex = -1, uv = -1, normal = -1;
	};

	/// <summary>
	/// Load an OBJ file.
	/// </summary>
	/// <param name="filepath">The file path</param>
	/// <param name="pts">output vector of the points of the OBJ file</param>
	/// <param name="normals"> output vector of the normals of the OBJ file</param>
	/// <param name="uvs">output vector of the uvs of the OBJ file</param>
	/// <param name="faces">output vector of the faces of the OBJ file</param>
	static void ObjLoader(std::string filepath, std::vector<glm::vec3>* pts, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uvs, std::vector<std::vector<int>>* faces) {
		if (global.debug_file_load)
			printf("Load : %s\n", filepath.c_str());
		std::ifstream file(filepath);
		if (!file.is_open()) return; //if openning failed, return

		std::vector<std::vector<PointFaceData>> tmpFaces;
		std::vector<glm::vec3> normalPerVertex;
		std::vector<glm::vec3> tmpN;
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> tmp = SplitString(line, " ");
			if (tmp[0].find_first_of("#") == -1) {
				if (tmp[0].compare("v") == 0) {
					pts->push_back(glm::vec3(std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3])));
					normalPerVertex.push_back(glm::vec3(0));
				}
				else if (tmp[0].compare("vt") == 0) {
					uvs->push_back(glm::vec2(std::stod(tmp[1]), std::stod(tmp[2])));
				}
				else if (tmp[0].compare("vn") == 0) {
					tmpN.push_back(glm::vec3(std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3])));
				}
				else if (tmp[0].compare("f") == 0) {
					std::vector<PointFaceData> ids;
					for (int i = 1; i < tmp.size(); i++) {
						if (tmp[i].find_first_of("#") == -1) {
							std::vector<std::string> fTmp = SplitString(tmp[i], "/");
							PointFaceData pfd;

							if (fTmp.size() >= 1) {
								pfd.vertex = std::stoi(fTmp[0])-1;
								if (fTmp.size() >= 2) {
									pfd.uv = std::stoi(fTmp[1])-1;
									if (fTmp.size() >= 3) {
										pfd.normal = std::stoi(fTmp[2])-1;
									}
								}
							}
							ids.push_back(pfd);
						}
						else {
							i = tmp.size();
						}
					}
					if (ids.size() >= 3) {
						tmpFaces.push_back(ids);
					}
				}
			}
		}
		int cursor = 0;
		for(size_t i = 0, maxI = tmpFaces.size(); i < maxI; i++){
			std::vector<PointFaceData> pfds = tmpFaces[i];
			std::vector<int> facesIds;
			for(size_t j = 0, maxJ = pfds.size(); j < maxJ; j++){
				facesIds.push_back(pfds[j].vertex);
				normalPerVertex[pfds[j].vertex] += tmpN[pfds[j].normal];
				cursor++;
			}
			faces->push_back(facesIds);
		}

		for (size_t i = 0, maxI = normalPerVertex.size(); i < maxI; i++) {
			normals->push_back(glm::normalize(normalPerVertex[i]));
		}
				

	}

	/// <summary>
	/// Get the extension of the file, in lowercase.
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <returns>The extension of the file, in lowercase.</returns>
	static std::string GetExtensionLower(std::string filename) {
		std::string ext = filename.substr(filename.find_last_of(".") + 1);
		std::string res = "";
		for (int i = 0, max = ext.size(); i < max; i++) {
			res += tolower(ext.at(i));
		}
		return res;
	}

	/// <summary>
	/// Trim a string.
	/// </summary>
	/// <param name="str">The string to trim</param>
	/// <returns>The trimmed string.</returns>
	static std::string trim(std::string str){
		if(str.size() <= 0) return "";
		int leftPosTrim = 0;
		int rightPosTrim = str.size();
		bool leftPosEnd = false;
		bool rightPosEnd = false;
		for(int i = 0, max = (((float)str.size())/2.0f) + 1; i < max && (!leftPosEnd || !rightPosEnd); i++){
			if(!leftPosEnd){
				if(str.at(i) == ' '){
					leftPosTrim = i+1;
				} else {
					leftPosEnd = true;
				}
			}
			if(!rightPosEnd){

				if(str.at(str.size() - 1 - i) == ' '){
					rightPosTrim = str.size() - i -1;
				} else {
					rightPosEnd = true;
				}
			}
		}
		int length = rightPosTrim - leftPosTrim;
		if(length > 0){
			return str.substr(leftPosTrim, length);
		} 
		return "";
	}
};

#endif // !__TOOLS_HPP__
