#include "Widget.hpp"
#include <Render/Widget/WidgetOverlay.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Core/GLUtils.hpp"
#include "Render.module.hpp"

ImVec2 toImVec(const Vector2 &vec) { return ImVec2(vec.x, vec.y); }

void WidgetContainer::beginDraw() {
	ImGui::Begin(getLabel().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}
void WidgetContainer::render(const double &deltaTime) {
	if (getVisibility() == EVisibility::hidden)
		return;

	beginDraw();
	for (auto &i : items) {
		i->render(deltaTime);
	}
	endDraw();
}
void WidgetContainer::endDraw() {
	ImGui::End();
}
WidgetContainer &WidgetContainer::operator<<(Widget *item) {
	if (item->container)
		item->container->operator>>(item);
	items.push_back(item);
	item->container = this;
	return *this;
}
WidgetContainer &WidgetContainer::operator>>(Widget *item) {
	if (this != item->container)
		return *this;

	items.remove(item);
	item->container = nullptr;
	return *this;
}
WidgetContainer &WidgetContainer::operator<<(std::list<Widget *> items){
	for (auto i : items) {
		(*this) << i;
	}
	return *this;
}
WidgetContainer &WidgetContainer::operator>>(std::list<Widget *> items) {
	for (auto i : items) {
		(*this) >> i;
	}
	return *this;
}
Widget *WidgetContainer::operator[](const string &label) {
	for (auto &i : items) {
		if (i->getLabel() == label) {
			return i;
		}
	}
	return nullptr;
}

UserWidget::UserWidget() {
}
void UserWidget::show() {
	if (getVisibility() == EVisibility::visible)
		return;
	RenderModule::instance()->getWidgetOverlay() << this;
	setVisibility(EVisibility::visible);
}
void UserWidget::hide() {
	if (getVisibility() == EVisibility::hidden)
		return;
	RenderModule::instance()->getWidgetOverlay() >> this;
	setVisibility(EVisibility::hidden);
}

namespace Clay {
	void Text::render(const double &deltaTime) {
		ImGui::Text(text.c_str());
	}
	void Text::setText(string newText) {
		string old = text;
		text = newText;
		onTextUpdated.broadcast(this, old, text);
	}

	void InputText::render(const double &deltaTime) {
		const auto InputCallback = [](ImGuiInputTextCallbackData *data)->int {
			InputText *self = static_cast<InputText *>(data->UserData);
			switch (data->EventFlag) {
				case ImGuiInputTextFlags_CallbackCompletion: {
					break;
				}
				case ImGuiInputTextFlags_CallbackHistory: {
					break;
				}
			}
			return 0;
		};
		const ImGuiInputTextFlags inFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

		if (ImGui::InputText(getLabel().c_str(), inBuffer, IM_ARRAYSIZE(inBuffer), inFlags, InputCallback, this)) {
			string old = text;
			text = string(inBuffer);
			ImGui::SetKeyboardFocusHere(-1);
			strcpy_s(inBuffer, "");
			onTextUpdated.broadcast(this, old, text);
		}
	}

	void Button::render(const double &deltaTime) {
		ImGui::Button(getLabel().c_str());
		if (ImGui::IsItemClicked(0)) {
			state = !state;
			onClick.broadcast(this, state);
		}
	}

	void ProgressBar::render(const double &deltaTime) {
		ImGui::ProgressBar(getFraction(), toImVec(getSize()));
	}
	void ProgressBar::setValue(float newValue) {
		value = std::fmax(newValue, 0.f);
		value = std::fmin(value, maxValue);
	}
	void ProgressBar::setMaxValue(float newValue) {
		newValue = std::fmax(newValue, 1.f);
		value = std::fmin(value, newValue);
	}

	Histogram::Histogram() {
		valuesCount = 20;
		values = new float[20];
	}
	Histogram::~Histogram() {
		valuesCount = 0;
		delete[] values;
	}
	void Histogram::render(const double &deltaTime) {
		ImGui::PlotHistogram(getLabel().c_str(), values, valuesCount, 0, nullptr, -1.f, 1.f, toImVec(getSize()));
	}

	void Histogram::pushValue(float newValue) {
		for (int i = 0; i < valuesCount - 1; ++i)
			values[i] = values[i + 1];
		values[valuesCount - 1] = newValue;
	}
};