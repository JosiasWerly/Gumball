#include "Mesh.hpp"


bool MeshFactory::assemble(Object *&content, Archive& ar) {
	vector<MeshVertexData> vertexData;
	vector<unsigned int> index;
	if (MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), vertexData, index)) {
		content = new MeshData(vertexData, index);
		return true;
	}
	return false;
}

bool MeshFactory::disassemble(Object *&content, Archive &ar) {
	delete content;
	content = nullptr;
	return true;
}