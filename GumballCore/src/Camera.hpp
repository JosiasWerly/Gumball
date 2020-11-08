#ifndef _camera
#define _camera
#include "Transform.hpp"
class ViewMode {
public:
    //projection = false ?//just for testing
    //    glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
    //    glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);

    enum class eProjectionMode {
        Ortho, Perspective, Custom
    };
    eProjectionMode eProjection;
    glm::mat4 mProjection = glm::mat4(1.0f);

    void setProjectionPerspective(float fovy, float aspect, float near = 0.1f, float far = 200.0f) {
        mProjection = glm::perspective(fovy, aspect, near, far);
        eProjection = eProjectionMode::Perspective;
    }
    void setProjectionOrtho(float left, float right, float bottom, float top, float near = .1f, float far = 200.0f) {
        mProjection = glm::ortho(left, right, bottom, top, near, far);
        eProjection = eProjectionMode::Perspective;
    }
    void setProjectionCustom(glm::mat4 customProj) {
        mProjection = customProj;
        eProjection = eProjectionMode::Custom;       
    }
    const glm::mat4& getProjection() {
        return mProjection;
    }
};
class Camera {
    unsigned char renderLayer;
public:
    Transform transform;
    ViewMode viewMode;

    Camera() {
    }
    unsigned char layer() {
        return renderLayer;
    }
    unsigned char layer(unsigned char newLayer) {
        return renderLayer = newLayer;
    }
};
#endif // !_camera
