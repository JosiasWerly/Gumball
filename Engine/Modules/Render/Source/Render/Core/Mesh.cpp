#include "Mesh.hpp"
#include "GLBuffer.hpp"

#include <map>
#include <regex>
#include <map>
#include <string>
using namespace std;


Inline void MeshInstance::bind() const { drawable->bind(); }
Inline void MeshInstance::unbind() const { drawable->unbind(); }
Inline void MeshInstance::draw() const { drawable->draw(); }
Inline MeshData *MeshInstance::getMeshData() { return meshData; }

struct gPackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const gPackedVertex that) const {
		return memcmp((void *)this, (void *)&that, sizeof(gPackedVertex)) > 0;
	};
};
bool getSimilarVertexIndex_fast(
	gPackedVertex &packed,
	std::map<gPackedVertex, unsigned int> &VertexToOutIndex,
	unsigned int &result
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

void CalculateIndexData(
	std::vector<glm::vec3> &in_vertices,
	std::vector<glm::vec2> &in_uvs,
	std::vector<glm::vec3> &in_normals,

	std::vector<unsigned> &out_indices,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals
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

bool LoadObjFile(
	const char *path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals) {

	auto splice = [](string &data, string lim, int begin = 0)->std::vector<std::string> {
		std::regex ws_re(lim);
		std::vector<std::string> result{
			std::sregex_token_iterator(data.begin() + begin, data.end(), ws_re, -1), {}
		};
		return result;
		};

	std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices, temp_normals;
	std::vector<glm::vec2> temp_uvs;

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
					auto &sData = rPack[i];
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

AssetFactory<MeshData>::AssetFactory() {
	extensions = { "obj" };
}
bool AssetFactory<MeshData>::load(Archive &ar, MeshData &val) {
	std::string path = ar.getFilePath().getPath().c_str();
	ar.close();
	vector<MeshVertexData> &meshBuffer = val.mesh;
	vector<unsigned> &index = val.index;
	{
		vector<glm::fvec3> vertex, normal;
		vector<glm::fvec2> uv;

		if (LoadObjFile(path.c_str(), vertex, uv, normal)) {
			vector<glm::fvec3> out_vertex, out_normal;
			vector<glm::fvec2> out_uv;
			CalculateIndexData(vertex, uv, normal, index, out_vertex, out_uv, out_normal);
			vertex = out_vertex;
			uv = out_uv;
			normal = out_normal;

			for (size_t i = 0; i < vertex.size(); i++) {
				meshBuffer.push_back({ vertex[i], normal[i], uv[i] });
			}
			return true;
		}
	}
	return false;
}
bool AssetFactory<MeshData>::save(Archive &ar, const MeshData &val) {
	throw;
	return false;
}

MeshInstance::MeshInstance() {
	drawable = new DrawableBuffer;
}
MeshInstance::~MeshInstance() {
	delete drawable;
}
void MeshInstance::setMeshData(MeshData *NewMeshData) {
	meshData = NewMeshData;
	drawable->bindAll();
	VboBuilder()
		.setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
		.addAttrib<float>(3)//pos
		.addAttrib<float>(3)//normal
		.addAttrib<float>(2)//uv
		.build();
	auto &ibo = drawable->getIbo();
	ibo.setBuffer(
		meshData->index.data(),
		static_cast<unsigned int> (meshData->index.size() * sizeof(unsigned)));
	drawable->unbindAll();
}