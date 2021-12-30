#include "Shaders.hpp"


bool ShaderFactory::assemble(Asset &asset, Archive &ar) {
	if (int sh = ShaderFunctionsLibrary::loadFromArchive(ar) > 0) {
		Shader *newShader = new Shader;
		asset << newShader;
		return true;
	}
	return false;
}