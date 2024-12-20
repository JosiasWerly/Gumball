#include "Glyph.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "Core/GLUtils.hpp"
using namespace Glyph;

ImVec2 toImVec(const Vector2 &vec) { return ImVec2(vec.x, vec.y); }

Container::Container() :
	container(this),
	layout(this, eLayout::vertical) {
}
void Container::render() {
	if (ImGui::TreeNode(label().c_str())) {
		switch (layout()) {
			case eLayout::vertical:
				for (auto &w : container.getArr()) {
					w->render();
				}
				break;
			case eLayout::horizontal:
				auto it = container.getArr().begin();
				(*it)->render();
				for (; it != container.getArr().end(); ++it) {
					ImGui::SameLine();
					(*it)->render();
				}
				break;
		}
		ImGui::TreePop();
	}
}

void Button::render() {
	if (ImGui::Button(label().c_str())) {
		onClick.invoke(this);
	}
}

CheckBox::CheckBox() :
	state(this, false) {
}
void CheckBox::render() {
	if (ImGui::Checkbox(label().c_str(), &state())) {
		state.notify.invoke(this, !state(), state());
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
	if (ImGui::BeginCombo(label().c_str(), selectedIndex == -1 ? "" : items[selectedIndex].c_str(), 0)) {
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

Text::Text() :
	text(this, "") {
}
void Text::render() {
	ImGui::TextWrapped(text().c_str());
	ImGui::Spacing();
}

int TextInput::callback(ImGuiInputTextCallbackData *data) {
	TextInput *self = static_cast<TextInput *>(data->UserData);
	switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackResize:
			self->text().resize(data->BufTextLen);
			data->Buf = const_cast<char *>(self->text().c_str());
			break;
		case ImGuiInputTextFlags_CallbackCompletion:
			//self->onTextUpdated.broadcast(self, self->text());
			break;
	}
	return 0;
}
TextInput::TextInput() :
	text(this, " "),
	multiline(this, true),
	size(this, { 100, 40 }) {
}
void TextInput::render() {
	bool edited = false;
	if (multiline()) {
		edited = ImGui::InputTextMultiline(
			label().c_str(),
			const_cast<char *>(text().c_str()), text().size() + 1,
			toImVec(size()),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine |
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCompletion,
			TextInput::callback,
			this
		);
	}
	else {
		edited = ImGui::InputText(
			label().c_str(),
			const_cast<char *>(text().c_str()), text().size() + 1,
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
	if (ImGui::ColorEdit4(label().c_str(), color, ImGuiColorEditFlags_Uint8)) {
		;
	}
}
void ColorPicker::setColor(Color color) {
	this->color[0] = static_cast<float>(color.r) / 255.f;
	this->color[1] = static_cast<float>(color.g) / 255.f;
	this->color[2] = static_cast<float>(color.b) / 255.f;
	this->color[3] = static_cast<float>(color.a) / 255.f;
}
Color ColorPicker::getColor() const {
	return {
		static_cast<unsigned char>(color[0] * 255.f),
		static_cast<unsigned char>(color[1] * 255.f),
		static_cast<unsigned char>(color[2] * 255.f),
		static_cast<unsigned char>(color[3] * 255.f)
	};
}

ProgressBar::ProgressBar() :
	size(this, { 150, 20 }) {
}
void ProgressBar::render() {
	ImGui::ProgressBar(getFraction(), toImVec(size()));
}
void ProgressBar::setValue(float newValue) {
	value = std::fmax(newValue, 0.f);
	value = std::fmin(value, maxValue);
}
void ProgressBar::setMaxValue(float newValue) {
	newValue = std::fmax(newValue, 1.f);
	value = std::fmin(value, newValue);
}

Histogram::Histogram() :
	size(this, { 150, 100 }) {
	valuesCount = 20;
	values = new float[20];
}
Histogram::~Histogram() {
	valuesCount = 0;
	delete[] values;
}
void Histogram::render() {
	ImGui::PlotHistogram(label().c_str(), values, valuesCount, 0, nullptr, -1.f, 1.f, toImVec(size()));
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
	ImGui::InputScalarN(label().c_str(), ImGuiDataType_S32, pValues, pValuesSize, NULL, NULL, "%d", 0);
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
	ImGui::InputScalarN(label().c_str(), ImGuiDataType_Float, pValues, pValuesSize, NULL, NULL, "%.2f", 0);
}
//ImGui::InputScalarN(getLabel().c_str(), ImGuiDataType_Float, pValues, pValuesSize, NULL, NULL, "%.2f", 0);
//DragScalarN(label, ImGuiDataType_S32, v, 2, v_speed, &v_min, &v_max, format, flags)
//SliderScalarN(label, ImGuiDataType_S32, v, 2, &v_min, &v_max, format, flags);