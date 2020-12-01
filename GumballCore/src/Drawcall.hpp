#ifndef _drawCall
#define _drawCall
#include "GFunctionLibrary.hpp"
#include "glm/glm.hpp"
#include "GLBuffers.hpp"
#include "Shader.hpp"



#include "stb_image.h"
//old stuff
struct TextureReference {
    unsigned int glBufferId;
    unsigned char* memoryBuffer;
    int width, height;
};
class TextureFactory :
    public iAssetFactory {
public:
    bool canBuild(const string& filePath) {
        return getExtOfFilePath(filePath) == "png";
    }
    bool loadFromDisk(const string& filePath, AssetContent& content) {
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

            content.anew<TextureReference>() = { bufferId, imageBuffer, x, y };
            return true;
		}
        return false;
    }
    bool unLoad(AssetContent& content) {
        TextureReference& dt = content.get<TextureReference>();
        glDeleteTextures(1, &dt.glBufferId);
        delete dt.memoryBuffer;
        content.free();
        return true;
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
        //TODO
        //textureData = TextureSystem::instance().getAsset(name);
    }
    void bind(char slot = 0) {
        glDCall(glActiveTexture(GL_TEXTURE0 + slot));
        glDCall(glBindTexture(GL_TEXTURE_2D, textureData.glBufferId));
    }
    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
//end of oldstuff

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
