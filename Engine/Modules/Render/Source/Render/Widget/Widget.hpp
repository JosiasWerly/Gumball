#pragma once
#ifndef __widget
#define __widget
#include <Gumball/Event.hpp>
#include "WidgetOverlay.hpp"

struct ImGuiInputTextCallbackData;
class Widget;
class IWidgetContainer;

typedef std::list<Widget *> Widgets;

enum class eVisibility {
	visible, hidden
};

enum class eLayout {
	vertical, horizontal
};

enum class eInputControl {
	input, drag, slider
};

class GMODULE Widget {
	friend class IWidgetContainer;
	friend class WidgetContainer;
	friend class UserWidget;

	string label;
	Vector2 size = Vector2(0, 0);
	eVisibility visibility = eVisibility::hidden;

	IWidgetContainer *container = nullptr;
protected:
	virtual void render() = 0;

public:
	Dispatcher<Widget *, bool> onClick;
	Dispatcher<Widget *, eVisibility> onVisibilityChange;

	Widget();
	virtual ~Widget() = default;

	void setLabel(string label);
	void setSize(Vector2 size);
	void setVisibility(eVisibility visibility);

	Inline const string &getLabel() const { return label; }
	Inline Vector2 getSize() const { return size; }
	Inline eVisibility getVisibility() const { return visibility; }
	Inline IWidgetContainer *getContainer() const { return container; }
};

class GMODULE IWidgetContainer : public Widget {
protected:
	eLayout layout = eLayout::vertical;
	Widgets widgets;

	virtual void render() = 0;

public:
	IWidgetContainer() = default;
	virtual ~IWidgetContainer();

	void setLayout(eLayout layout) { this->layout = layout; }
	Inline eLayout getLayout() { return layout; }

	IWidgetContainer &operator<<(Widget *widget);
	IWidgetContainer &operator>>(Widget *widget);
	IWidgetContainer &operator<<(Widgets widgets);
	IWidgetContainer &operator>>(Widgets widgets);
	const Widget *operator[](const string &label);
	Inline const Widgets &getWidgets() const { return widgets; };
	Inline bool hasChild(Widget *item) const { return std::find(widgets.begin(), widgets.end(), item) != std::end(widgets); };
};

class GMODULE WidgetContainer : public IWidgetContainer {
	void render();
public:
};

class GMODULE UserWidget : public IWidgetElement, public IWidgetContainer {
	void onShowEvent(Widget * , eVisibility);	
	void render();

public:
	UserWidget();
	~UserWidget();
};


namespace Glyph {
	class Button;
	class CheckBox;
	class Combo;
	class Text;
	class TextInput;
	class ColorPicker;
	class ProgressBar;
	class Histogram;
	
	class ControlInputData {};
	class IntergerInput;
	class FloatInput;
};

class GMODULE Glyph::Button : public Widget {
	bool state = false;

	void render();

public:
	Inline bool getState() const { return state; }
};

class GMODULE Glyph::CheckBox : public Widget {
	bool state = false;

	void render();

public:
	Inline bool getState() const { return state; }
};

class GMODULE Glyph::Combo : public Widget {
	int selectedIndex = -1;
	std::vector<string> items;
	
	void render();

public:
	void setItems(std::vector<string> items);
	void setSelectedIndex(int index);

	const std::vector<string> &getItems() const { return items; }
	int getSelectedIndex() const { return selectedIndex; }
	const string *getSelectedItem() const { return selectedIndex != -1 ? &items[selectedIndex] : nullptr; }
};

class GMODULE Glyph::Text : public Widget {
	string text = "Text";

	void render();

public:
	void setText(string newText);
	const string& getText() const { return text; }

	Dispatcher<Widget *, string, string> onTextUpdated;
};

class GMODULE Glyph::TextInput : public Widget {
	friend int callback(ImGuiInputTextCallbackData *data);
	bool isMultiline = false;
	string text;

	void render();
	static int callback(ImGuiInputTextCallbackData *data);

public:
	void setMultiline(bool multiline) { this->isMultiline = multiline; }
	void setText(string text);	
	bool getMultiline() const { return isMultiline; }
	const string &getText() const { return text; }
	
	Dispatcher<Widget *, const string&> onTextUpdated;
};

class GMODULE Glyph::ColorPicker : public Widget {
	float color[4];
	void render();

public:
	ColorPicker();
	void setColor(Color);
	Color getColor() const;
};

class GMODULE Glyph::ProgressBar : public Widget {
	float value = 0.f, maxValue = 100.f;

	void render();

public:
	ProgressBar();
	void setValue(float newValue);
	void setMaxValue(float newValue);
	float getValue() const { return value; }
	float getMaxValue() const { return maxValue; }
	float getFraction() const { return value / maxValue; }
	bool atMax() const { return value == maxValue; }
};

class GMODULE Glyph::Histogram : public Widget {
	int valuesCount = 20;
	float *values;

	void render();

public:
	Histogram();
	~Histogram();
	void pushValue(float newValue);
};

class GMODULE Glyph::IntergerInput : public Widget {
	int *pValues = nullptr;
	unsigned pValuesSize = 0;

	void render();

public:
	IntergerInput(unsigned size);
	~IntergerInput();
};

class GMODULE Glyph::FloatInput : public Widget {
	float *pValues = nullptr;
	unsigned pValuesSize = 0;

	void render();

public:
	FloatInput(unsigned size);
	~FloatInput();
};

#endif // !__widget