#include "Widget.hpp"
#include <Render/Widget/WidgetOverlay.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Core/GLUtils.hpp"
#include "Render.module.hpp"

ImVec2 toImVec(const Vector2 &vec) { return ImVec2(vec.x, vec.y); }

Widget::Widget() :
	//label(std::string("##") + std::to_string((int)this)) 
	label(std::to_string((int)this))
{}
void Widget::setLabel(string label) {
	this->label = label;
}
void Widget::setSize(Vector2 size) {
	this->size = size; 
}
void Widget::setVisibility(eVisibility visibility) {
	if (this->visibility == visibility)
		return;

	this->visibility = visibility;
	onVisibilityChange.broadcast(this, this->visibility);
}

IWidgetContainer::~IWidgetContainer() {
	for (auto w : widgets) {
		delete w;
	}
	widgets.clear();
}
IWidgetContainer &IWidgetContainer::operator<<(Widget *widget) {
	if (widget->container)
		widget->container->operator>>(widget);
	widgets.push_back(widget);
	widget->container = this;
	return *this;
}
IWidgetContainer &IWidgetContainer::operator>>(Widget *widget) {
	if (this != widget->container)
		return *this;

	widgets.remove(widget);
	widget->container = nullptr;
	return *this;
}
IWidgetContainer &IWidgetContainer::operator<<(Widgets widgets) {
	for (auto w : widgets)
		(*this) << w;
	return *this;
}
IWidgetContainer &IWidgetContainer::operator>>(Widgets widgets) {
	for (auto w : widgets)
		(*this) >> w;
	return *this;
}
const Widget *IWidgetContainer::operator[](const string &label) {
	for (auto &i : widgets) {
		if (i->getLabel() == label) {
			return i;
		}
	}
	return nullptr;
}

void WidgetContainer::render() {
	if (ImGui::TreeNode(getLabel().c_str())) {
		switch (layout) {
			case eLayout::vertical:
				for (auto &w : widgets) {
					w->render();
				}
				break;
			case eLayout::horizontal:
				auto it = widgets.begin();
				(*it)->render();
				for (; it != widgets.end(); ++it) {
					ImGui::SameLine();
					(*it)->render();
				}
				break;
		}
		ImGui::TreePop();
	}
}

UserWidget::UserWidget() {
	onVisibilityChange.bindMethod(this, &UserWidget::onShowEvent);
	setSize({ 300, 400 });
}
UserWidget::~UserWidget() {
	setVisibility(eVisibility::hidden);
}
void UserWidget::onShowEvent(Widget *, eVisibility) {
	if (getVisibility() == eVisibility::visible)
		RenderModule::instance()->getWidgetOverlay() << this;
	else
		RenderModule::instance()->getWidgetOverlay() >> this;
}
void UserWidget::render() {
	ImGui::Begin(getLabel().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	for (auto &w : getWidgets()) {
		w->render();
	}
	ImGui::End();
}


using namespace Glyph;

void Button::render() {
	if (ImGui::Button(getLabel().c_str())) {
		state = !state;
		onClick.broadcast(this, state);
	}
}

void CheckBox::render() {
	if (ImGui::Checkbox(getLabel().c_str(), &state)) {
		onClick.broadcast(this, state);
	}
}

void Combo::setItems(std::vector<string> items) {
	this->items = items;
	selectedIndex = -1;
}
void Combo::setSelectedIndex(int index) {
	this->items = items;
}
void Combo::render() {
	if (ImGui::BeginCombo(getLabel().c_str(), selectedIndex == -1 ? "" : items[selectedIndex].c_str(), 0)) {
		for (int i = 0; i < items.size(); ++i) {
			const bool isSelected = (selectedIndex == i);
			if (isSelected)
				ImGui::SetItemDefaultFocus();

			if (ImGui::Selectable(items[i].c_str(), isSelected))
				selectedIndex = i;
		}
		ImGui::EndCombo();
	}
}

void Text::render() {
	ImGui::TextWrapped(text.c_str());
	ImGui::Spacing();
}
void Text::setText(string newText) {
	string old = text;
	text = newText;
	onTextUpdated.broadcast(this, old, text);
}

int TextInput::callback(ImGuiInputTextCallbackData *data) {
	TextInput *self = static_cast<TextInput *>(data->UserData);
	switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackResize:
			self->text.resize(data->BufTextLen);
			data->Buf = const_cast<char*>(self->text.c_str());
			break;
		case ImGuiInputTextFlags_CallbackCompletion:
			self->onTextUpdated.broadcast(self, self->text);
			break;
	}
	return 0;
}
void TextInput::setText(string text) {
	this->text = text;
}
void TextInput::render() {
	bool edited = false;
	if (isMultiline) {
		edited = ImGui::InputTextMultiline(
			getLabel().c_str(),
			const_cast<char *>(text.c_str()), text.size() + 1,
			toImVec(getSize()),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine |
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCompletion,
			TextInput::callback,
			this
		);

	}
	else {
		edited = ImGui::InputText(
			getLabel().c_str(),
			const_cast<char *>(text.c_str()), text.size() + 1,
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine |
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCompletion,
			TextInput::callback,
			this
		);
	}
}

ColorPicker::ColorPicker() {
	std::memset(color, 0, sizeof(float) * 4);
}
void ColorPicker::render() {
	if (ImGui::ColorEdit4(getLabel().c_str(), color, ImGuiColorEditFlags_Uint8)) {
		;
	}
}
void ColorPicker::setColor(Color color) {
	this->color[0] = static_cast<float>(color.r)/255.f;
	this->color[1] = static_cast<float>(color.g)/255.f;
	this->color[2] = static_cast<float>(color.b)/255.f;
	this->color[3] = static_cast<float>(color.a)/255.f;
}
Color ColorPicker::getColor() const {
	return {
		static_cast<unsigned char>(color[0]*255.f),
		static_cast<unsigned char>(color[1]*255.f),
		static_cast<unsigned char>(color[2]*255.f),
		static_cast<unsigned char>(color[3]*255.f)
	};
}

ProgressBar::ProgressBar() {
	setSize({ 150, 20 });
}
void ProgressBar::render() {
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
	setSize({ 150, 100 });
	valuesCount = 20;
	values = new float[20];
}
Histogram::~Histogram() {
	valuesCount = 0;
	delete[] values;
}
void Histogram::render() {
	ImGui::PlotHistogram(getLabel().c_str(), values, valuesCount, 0, nullptr, -1.f, 1.f, toImVec(getSize()));
}
void Histogram::pushValue(float newValue) {
	for (int i = 0; i < valuesCount - 1; ++i)
		values[i] = values[i + 1];
	values[valuesCount - 1] = newValue;
}

IntergerInput::IntergerInput(unsigned size) {
	pValuesSize = size;
	pValues = new int[size];
	std::memset(pValues, 0, size * sizeof(int));
}
IntergerInput::~IntergerInput() {
	delete pValues;
}
void IntergerInput::render() {	
	ImGui::InputScalarN(getLabel().c_str(), ImGuiDataType_S32, pValues, pValuesSize, NULL, NULL, "%d", 0);
}

FloatInput::FloatInput(unsigned size) {
	pValuesSize = size;
	pValues = new float[size];
	std::memset(pValues, 0, size * sizeof(float));
}
FloatInput::~FloatInput() {
	delete pValues;
}
void FloatInput::render() {
	ImGui::InputScalarN(getLabel().c_str(), ImGuiDataType_Float, pValues, pValuesSize, NULL, NULL, "%.2f", 0);
}
//ImGui::InputScalarN(getLabel().c_str(), ImGuiDataType_Float, pValues, pValuesSize, NULL, NULL, "%.2f", 0);
//DragScalarN(label, ImGuiDataType_S32, v, 2, v_speed, &v_min, &v_max, format, flags)
//SliderScalarN(label, ImGuiDataType_S32, v, 2, &v_min, &v_max, format, flags);