#include "Mesh.hpp"
bool MeshData::archiveLoad(Archive &ar) {
	return MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), mesh, index);
}
bool MeshData::archiveSave(Archive &ar) {
	return false;
}