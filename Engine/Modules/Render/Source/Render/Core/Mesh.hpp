#pragma once
#ifndef _mesh
#define _mesh

#include <Content/Asset.hpp>
#include <vector>

struct GMODULE MeshVertexData {
	glm::vec3 pos, normal;
	glm::vec2 uv;
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

class GMODULE MeshInstance {
	class DrawableBuffer *drawable = nullptr;
	MeshData *meshData = nullptr;
public:
	MeshInstance();
	~MeshInstance();
	void setMeshData(MeshData *NewMeshData);

	Inline void bind() const;
	Inline void unbind() const;
	Inline void draw() const;
	Inline MeshData *getMeshData();
};

template<> class AssetFactory<MeshData> : public TAssetFactory<MeshData> {
public:
	AssetFactory();
	bool load(Archive &ar, MeshData &val);
	bool save(Archive &ar, const MeshData &val);
};
#endif // !_mesh