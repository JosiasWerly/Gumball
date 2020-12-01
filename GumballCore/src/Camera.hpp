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

    void setProjectionPerspective(float fovy = glm::radians(45.0f), float aspect = 1.3333333730697632, float near = 0.1f, float far = 500.0f) {
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
public:
    Transform transform;
    ViewMode viewMode;

    Camera() {
        viewMode.setProjectionPerspective();
    }

};
#endif // !_camera
