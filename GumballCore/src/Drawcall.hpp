#ifndef _drawCall
#define _drawCall
#include "glm/glm.hpp"
#include "Gumball.hpp"
#include "GLBuffers.hpp"
#include "Shader.hpp"



#include "stb_image.h"
struct TextureReference {
    unsigned int glBufferId;
    unsigned char* memoryBuffer;
    int width, height;
};
class TextureSystem :
    public AssetFactory<TextureReference>,
    public Singleton<TextureSystem> {
public:
    void loadFromFile(string filePath) {
        stbi_set_flip_vertically_on_load(true);
        int x, y, channels;
        unsigned char* imageBuffer = stbi_load(filePath.c_str(), &x, &y, &channels, 4);
        unsigned int bufferId = 0;

        if (imageBuffer) {
            glDCall(glGenTextures(1, &bufferId));
            glDCall(glBindTexture(GL_TEXTURE_2D, bufferId));
            //filtering
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            //wrapping
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            glDCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer));
            glDCall(glBindTexture(GL_TEXTURE_2D, 0));
            assets.push(gbLib::getNameOfFilePath(filePath), { bufferId, imageBuffer, x, y });
        }
        else throw;
    }
    void unload(string name) {
        auto s = assets.get(name);
        glDeleteTextures(1, &s->glBufferId);
        delete s->memoryBuffer;
        assets.pop(name);
    }
    void unloadAll() {

    }
};
class Texture {
public:
    TextureReference textureData;
    Texture() {
    }
    Texture(string name) {
        changeTexture(name);
    }
    void changeTexture(string name) {
        textureData = TextureSystem::instance().getAsset(name);
    }
    void bind(char slot = 0) {
        glDCall(glActiveTexture(GL_TEXTURE0 + slot));
        glDCall(glBindTexture(GL_TEXTURE_2D, textureData.glBufferId));
    }
    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

class iDrawCall {
public:
    Shader sa;
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above
    virtual void draw(const class Renderer& renderer) = 0;
};
#endif // !_drawCall
