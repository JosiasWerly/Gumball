#include "Shader.hpp"

void ShaderParam::setStorage(unsigned int type) {
    if (paramStorage)
        delete paramStorage;
    paramStorage = ShaderFunctionsLibrary::reflectGLEnum(type);
}