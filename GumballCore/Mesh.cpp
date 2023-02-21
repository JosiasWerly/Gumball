#include "Mesh.hpp"
bool MeshData::archiveLoad(Archive &ar) {
	return MeshFunctionsLibrary::LoadMeshVertexData(ar.filePath().c_str(), mesh, index);
}
bool MeshData::archiveSave(Archive &ar) {
	return false;
}

MeshBuffer::MeshBuffer(MeshData *mesh) {
	vao = new Vao();
	vao->bind();
	vbo = new Vbo();
	vbo->bind();
	ibo = new Ibo();
	ibo->bind();
	meshData = mesh;

	VboBuilder()
		.setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
		.addAttrib<float>(3)//pos
		.addAttrib<float>(3)//normal
		.addAttrib<float>(2)//uv
		.build();

	ibo->setBuffer(
		meshData->index.data(),
		static_cast<unsigned int> (meshData->index.size() * sizeof(unsigned)));
	unbind();
}
MeshBuffer::~MeshBuffer() {
	vao->unbind();
	vbo->unbind();
	ibo->unbind();
	delete vao;
	delete vbo;
	delete ibo;
}
void MeshBuffer::bind() const {
	vao->bind();
}
void MeshBuffer::unbind() const {
	vao->unbind();
}
void MeshBuffer::draw() const {
	glDrawElements(GL_TRIANGLES, ibo->size, GL_UNSIGNED_INT, nullptr);
}