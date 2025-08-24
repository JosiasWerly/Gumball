#include "Widget.hpp"
#include <Render/Widget/WidgetOverlay.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Core/GLUtils.hpp"
#include "Render.module.hpp"

Widget::Widget() :
	label(this, std::to_string(reinterpret_cast<__int64>(this))) {
}
Widget::~Widget() {
}

UserWidget::UserWidget() :
	size(this, { 300, 400 }),
	container(this),
	visibility(this, eVisibility::hidden) {
	visibility.notify.bind({ this, &UserWidget::onShowEvent });
}
UserWidget::~UserWidget() {
	visibility(eVisibility::hidden);
}
void UserWidget::onShowEvent(const Widget *, const eVisibility &, const eVisibility &) {
	if (eVisibility() == eVisibility::visible)
		RenderModule::instance()->getWidgetOverlay() << this;
	else
		RenderModule::instance()->getWidgetOverlay() >> this;
}
void UserWidget::render() {
	ImGui::Begin(label().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	for (auto w : container.getArr())
		w->render();
	ImGui::End();
}