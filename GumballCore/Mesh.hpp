#pragma once
#ifndef _mesh
#define _mesh

#include <regex>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>

#include "AssetManager.hpp"
#include "GLBuffer.hpp"
#include "Math.hpp"
#include "FunctionLibrary.hpp"
#include "Patterns.hpp"


using namespace std;

typedef std::vector<glm::vec3> listVec3;
typedef std::vector<glm::vec2> listVec2;
typedef std::vector<unsigned int> listUInt;

//TODO: perhaps I should integrate this with VboBuilder
struct GBCORE MeshVertexData {
	glm::vec3 pos, normal;
	glm::vec2 uv;
};

class MeshFunctionsLibrary {
public:
	struct gPackedVertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		bool operator<(const gPackedVertex that) const {
			return memcmp((void*)this, (void*)&that, sizeof(gPackedVertex)) > 0;
		};
	};
	static bool getSimilarVertexIndex_fast(
		gPackedVertex& packed,
		std::map<gPackedVertex, unsigned int>& VertexToOutIndex,
		unsigned int& result
	) {
		std::map<gPackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
		if (it == VertexToOutIndex.end()) {
			return false;
		}
		else {
			result = it->second;
			return true;
		}
	}

	static void CalculateIndexData(
		listVec3& in_vertices,
		listVec2& in_uvs,
		listVec3& in_normals,

		listUInt& out_indices,
		listVec3& out_vertices,
		listVec2& out_uvs,
		listVec3& out_normals
	) {
		std::map<gPackedVertex, unsigned int> VertexToOutIndex;

		// For each input vertex
		for (unsigned int i = 0; i < in_vertices.size(); i++) {

			gPackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };


			// Try to find a similar vertex in out_XXXX
			unsigned int index;
			bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

			if (found) { // A similar vertex is already in the VBO, use it instead !
				out_indices.push_back(index);
			}
			else { // If not, it needs to be added in the output data.
				out_vertices.push_back(in_vertices[i]);
				out_uvs.push_back(in_uvs[i]);
				out_normals.push_back(in_normals[i]);
				unsigned int newindex = (unsigned int)out_vertices.size() - 1;
				out_indices.push_back(newindex);
				VertexToOutIndex[packed] = newindex;
			}
		}
	}

	static bool LoadObjFile(
		const char* path,
		listVec3& out_vertices,
		listVec2& out_uvs,
		listVec3& out_normals) {

		auto splice = [](string& data, string lim, int begin = 0)->std::vector<std::string> {
			std::regex ws_re(lim);
			std::vector<std::string> result{
				std::sregex_token_iterator(data.begin() + begin, data.end(), ws_re, -1), {}
			};
			return result;
		};

		listUInt vertexIndices, uvIndices, normalIndices;
		listVec3 temp_vertices, temp_normals;
		listVec2 temp_uvs;

		ifstream fileStream(path);
		if (fileStream.is_open()) {
			string line;
			while (std::getline(fileStream, line)) {
				if (line.find("v ") != -1) {
					auto r = splice(line, "\\s+");
					temp_vertices.push_back(
						glm::fvec3{
							std::stod(r[1]),
							std::stod(r[2]),
							std::stod(r[3])
						}
					);
				}
				else if (line.find("vt ") != -1) {
					auto r = splice(line, "\\s+");
					temp_uvs.push_back(
						glm::fvec2{
							std::stod(r[1]),
							std::stod(r[2])
						}
					);
				}
				else if (line.find("vn ") != -1) {
					auto r = splice(line, "\\s+");
					temp_normals.push_back(
						glm::fvec3{
							std::stod(r[1]),
							std::stod(r[2]),
							std::stod(r[3])
						}
					);
				}
				else if (line.find("f ") != -1) {
					//vertex/uv/normal
					auto rPack = splice(line, "\\s+", 2);
					for (short i = 0; i < rPack.size(); i++) {
						auto& sData = rPack[i];
						auto r = splice(sData, "\\/");
						out_vertices.push_back(temp_vertices[stoi(r[0]) - 1]);
						out_uvs.push_back(temp_uvs[stoi(r[1]) - 1]);
						out_normals.push_back(temp_normals[stoi(r[2]) - 1]);
					}
					//temp_vertices[vertexIndex - 1]
				}
			}

			return true;
		}
		return false;
	}

	static bool LoadMeshVertexData(
		const char* path,
		vector<MeshVertexData>& meshBuffer,
		listUInt& index) {

		vector<glm::fvec3> vertex, normal;
		vector<glm::fvec2> uv;

		if (MeshFunctionsLibrary::LoadObjFile(
			path,
			vertex, uv, normal)) {
			vector<glm::fvec3> out_vertex, out_normal;
			vector<glm::fvec2> out_uv;
			MeshFunctionsLibrary::CalculateIndexData(vertex, uv, normal, index, out_vertex, out_uv, out_normal);
			vertex = out_vertex;
			uv = out_uv;
			normal = out_normal;

			for (size_t i = 0; i < vertex.size(); i++) {
				meshBuffer.push_back({ vertex[i], normal[i], uv[i] });
			}
			return true;
		}
		return false;
	}
};

class GBCORE MeshBuffer {
	struct Vao *vao = nullptr;
	struct Vbo *vbo = nullptr;
	struct Ibo *ibo = nullptr;
	class MeshData *meshData = nullptr;
public:
	MeshBuffer(MeshData *mesh);
	~MeshBuffer();
	Inline void bind() const;
	Inline void unbind() const;
	Inline void draw() const;

	Inline MeshData *getMeshData() const { return meshData; }
};

class MeshData {
public:
	vector<MeshVertexData> mesh;
	vector<unsigned int> index;

	MeshData() = default;
	MeshData(const vector<MeshVertexData> &mesh, const vector<unsigned> &index) :
		mesh(mesh),
		index(index) {
	}
};

template<> class AssetFactory<MeshData> : public TAssetFactory<MeshData> {
public:
	AssetFactory();
	bool load(Archive &ar, MeshData &val);
	bool save(Archive &ar, const MeshData &val);
};
#endif // !_mesh