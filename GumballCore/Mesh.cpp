#include "Mesh.hpp"


bool MeshFactory::assemble(Asset& asset, Archive& ar) {
	vector<MeshVertexData> vertexData;
	vector<unsigned int> index;
	if (MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), vertexData, index)) {
		asset.setContent(new MeshData(vertexData, index));
		return true;
	}
	return false;
}

bool MeshFactory::disassemble(Asset &asset, Archive &ar) {
	return true;
}