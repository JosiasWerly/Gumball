#ifndef _mesh
#define _mesh

#include <regex>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstring>
#include <glm/glm.hpp>

namespace GMesh {

	struct PackedVec3 {
		glm::vec3 position;
		bool operator<(const PackedVec3 that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVec3)) > 0;
		};
	};
	bool getSimilarVec3_fast(
		PackedVec3& packed,
		std::map<PackedVec3, unsigned int>& VertexToOutIndex,
		unsigned int& result
	) {
		std::map<PackedVec3, unsigned int>::iterator it = VertexToOutIndex.find(packed);
		if (it == VertexToOutIndex.end()) {
			return false;
		}
		else {
			result = it->second;
			return true;
		}
	}
	void gumballIndexer(
		std::vector<glm::vec3>& in_vertices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<unsigned int>& out_indices) {
		std::map<PackedVec3, unsigned int> VertexToOutIndex;

		for (unsigned int i = 0; i < in_vertices.size(); i++) {
			PackedVec3 packed = { in_vertices[i] };
			unsigned int index;
			bool found = getSimilarVec3_fast(packed, VertexToOutIndex, index);

			if (found) {
				out_indices.push_back(index);
			}
			else {
				out_vertices.push_back(in_vertices[i]);
				unsigned int newindex = (unsigned int)out_vertices.size() - 1;
				out_indices.push_back(newindex);
				VertexToOutIndex[packed] = newindex;
			}
		}
	}
	
	bool gumballLoadObj(
		const char* path, 
		std::vector<glm::vec3>& out_vertices) {
		ifstream fileStream(path);

		bool v = false;
		std::vector<glm::vec3> tempVertex;
		if (fileStream.is_open()) {
			string line;
			while (std::getline(fileStream, line)){
				if (line.find("v ") != -1) {
					std::regex ws_re("\\s+");
					std::vector<std::string> result{
						std::sregex_token_iterator(line.begin(), line.end(), ws_re, -1), {}
					};
					tempVertex.push_back(
							glm::fvec3{
								std::stod(result[1]),
								std::stod(result[2]),
								std::stod(result[3])
							}
					);
					
				}
				else if (line.find("f ") != -1) {
					std::regex ws_re("\\s+");
					std::vector<std::string> data{
						std::sregex_token_iterator(line.begin()+2, line.end(), ws_re, -1), {}
					};
					for (short i = 0; i < data.size(); i++) {
						auto& sData = data[i];
						std::regex ws_rebar("\\/");
						std::vector<std::string> vertexData{
							std::sregex_token_iterator(sData.begin(), sData.end(), ws_rebar, -1), {}
						};
						out_vertices.push_back(tempVertex[stoi(vertexData[0])-1]);
					}
					//temp_vertices[vertexIndex - 1]
					

				}
			}

			return true;
		}
		return false;
	}
	//bool loadOBJ(
	//	const char* path,
	//	std::vector<glm::vec3>& out_vertices,
	//	std::vector<glm::vec2>& out_uvs,
	//	std::vector<glm::vec3>& out_normals
	//) {
	//	printf("Loading OBJ file %s...\n", path);
	//	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	//	std::vector<glm::vec3> temp_vertices;
	//	std::vector<glm::vec2> temp_uvs;
	//	std::vector<glm::vec3> temp_normals;
	//	FILE* file;
	//	fopen_s(&file, path, "r");
	//	if (file == NULL) {
	//		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
	//		return false;
	//	}
	//	while (1) {
	//		char lineHeader[128];
	//		// read the first word of the line
	//		int res = fscanf_s(file, "%s", lineHeader);
	//		if (res == EOF)
	//			break; // EOF = End Of File. Quit the loop.
	//		// else : parse lineHeader
	//		if (strcmp(lineHeader, "v") == 0) {
	//			glm::vec3 vertex;
	//			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	//			temp_vertices.push_back(vertex);
	//		}
	//		else if (strcmp(lineHeader, "vt") == 0) {
	//			glm::vec2 uv;
	//			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
	//			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
	//			temp_uvs.push_back(uv);
	//		}
	//		else if (strcmp(lineHeader, "vn") == 0) {
	//			glm::vec3 normal;
	//			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
	//			temp_normals.push_back(normal);
	//		}
	//		else if (strcmp(lineHeader, "f") == 0) {
	//			std::string vertex1, vertex2, vertex3;
	//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	//			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
	//			if (matches != 9) {
	//				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
	//				return false;
	//			}
	//			vertexIndices.push_back(vertexIndex[0]);
	//			vertexIndices.push_back(vertexIndex[1]);
	//			vertexIndices.push_back(vertexIndex[2]);
	//			uvIndices.push_back(uvIndex[0]);
	//			uvIndices.push_back(uvIndex[1]);
	//			uvIndices.push_back(uvIndex[2]);
	//			normalIndices.push_back(normalIndex[0]);
	//			normalIndices.push_back(normalIndex[1]);
	//			normalIndices.push_back(normalIndex[2]);
	//		}
	//		else {
	//			// Probably a comment, eat up the rest of the line
	//			char stupidBuffer[1000];
	//			fgets(stupidBuffer, 1000, file);
	//		}
	//	}
	//	// For each vertex of each triangle
	//	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
	//		// Get the indices of its attributes
	//		unsigned int vertexIndex = vertexIndices[i];
	//		unsigned int uvIndex = uvIndices[i];
	//		unsigned int normalIndex = normalIndices[i];
	//		// Get the attributes thanks to the index
	//		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
	//		glm::vec2 uv = temp_uvs[uvIndex - 1];
	//		glm::vec3 normal = temp_normals[normalIndex - 1];
	//		// Put the attributes in buffers
	//		out_vertices.push_back(vertex);
	//		out_uvs.push_back(uv);
	//		out_normals.push_back(normal);
	//	}
	//	return true;
	//}
};
#endif // !_mesh
