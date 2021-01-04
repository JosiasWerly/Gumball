#include "Renderer.hpp"


void IRenderDisposer::disposeRender() {
    for (auto c : renderRef->views) {
        cameraModelCache = c->transform.getModel();
        for (auto& d : renderRef->drawcalls) {
            d->sa.params.get<glm::mat4>("uView") = cameraModelCache;
            d->sa.params.get<glm::mat4>("uProj") = c->viewMode.mProjection;
            d->sa.params.get<glm::mat4>("uModel") = d->transform->getResultModel();
            d->draw(*this);
        }
    }
}