#include "Mesh.hpp"

AssetFactory<MeshData>::AssetFactory() {
	extensions = { "obj" };
}
bool AssetFactory<MeshData>::load(Archive &ar, MeshData &val) {
	return MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), val.mesh, val.index);
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