#include "Mesh.hpp"
bool MeshData::archiveLoad(Archive &ar) {
	vector<MeshVertexData> vertexData;
	vector<unsigned int> index;
	if (MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), vertexData, index)) {
		mesh = vertexData;
		index = index;
		return true;
	}
	return false;
}
bool MeshData::archiveSave(Archive &ar) {
	return false;
}