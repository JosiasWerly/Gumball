#pragma once
#ifndef __widget
#define __widget
#include <Gumball/Event.hpp>
#include "WidgetOverlay.hpp"

class Widget;
class WidgetContainer;

typedef std::list<Widget *> Widgets;

enum class eVisibility {
	visible, hidden
};

enum class eLayout {
	vertical, horizontal
};

class GMODULE Widget {
	friend class WidgetContainer;

	string label = "widget";
	Vector2 size = Vector2(0, 0);
	eVisibility visibility = eVisibility::hidden;

	WidgetContainer *container = nullptr;
protected:
	virtual void render(const double &deltaTime) = 0;

public:
	Dispatcher<Widget *, bool> onClick;
	Dispatcher<Widget *, eVisibility> onVisibilityChange;

	Widget() = default;
	virtual ~Widget() = default;

	void setLabel(string label);
	void setSize(Vector2 size);
	void setVisibility(eVisibility visibility);

	Inline const string &getLabel() const { return label; }
	Inline Vector2 getSize() const { return size; }
	Inline eVisibility getVisibility() const { return visibility; }
	Inline WidgetContainer *getContainer() const { return container; }
};

class GMODULE WidgetContainer : public Widget {
private:
	eLayout layout = eLayout::vertical;
	Widgets widgets;

protected:
	void render(const double &deltaTime);

public:
	WidgetContainer() = default;
	virtual ~WidgetContainer();

	void setLayout(eLayout layout) { this->layout = layout; }
	Inline eLayout getLayout() { return layout; }

	WidgetContainer &operator<<(Widget *widget);
	WidgetContainer &operator>>(Widget *widget);
	WidgetContainer &operator<<(Widgets widgets);
	WidgetContainer &operator>>(Widgets widgets);
	Widget *operator[](const string &label);
	Inline bool hasChild(Widget *item) const { return std::find(widgets.begin(), widgets.end(), item) != std::end(widgets); };
};

class GMODULE UserWidget : public IWidgetElement, public WidgetContainer {
	void onShowEvent(Widget * , eVisibility);	
	void render(const double &deltaTime);

public:
	UserWidget();
	~UserWidget();
};

struct ImGuiInputTextCallbackData;
namespace Glyph {
	class Button;
	class CheckBox;
	class Combo;
	class Text;
	class TextInput;
	class ControlInput; //TODO
	class ColorPicker;
	class ProgressBar;
	class Histogram;
};

class GMODULE Glyph::Button : public Widget {
	bool state = false;

	void render(const double &deltaTime);

public:
	Inline bool getState() const { return state; }
};

class GMODULE Glyph::CheckBox : public Widget {
	bool state = false;

	void render(const double &deltaTime);

public:
	Inline bool getState() const { return state; }
};

class GMODULE Glyph::Combo : public Widget {
	int selectedIndex = -1;
	std::vector<string> items;
	
	void render(const double &deltaTime);

public:
	void setItems(std::vector<string> items);
	void setSelectedIndex(int index);

	const std::vector<string> &getItems() const { return items; }
	int getSelectedIndex() const { return selectedIndex; }
	const string *getSelectedItem() const { return selectedIndex != -1 ? &items[selectedIndex] : nullptr; }
};

class GMODULE Glyph::Text : public Widget {
	string text = "Text";

	void render(const double &deltaTime);
public:
	Dispatcher<Widget *, string, string> onTextUpdated;

	void setText(string newText);
	const string& getText() const { return text; }
};

class GMODULE Glyph::TextInput : public Widget {
	friend int callback(ImGuiInputTextCallbackData *data);

	string text;

	void render(const double &deltaTime);
	static int callback(ImGuiInputTextCallbackData *data);
public:
	Dispatcher<Widget *, const string&> onTextUpdated;

	void setText(string text);
	const string &getText() const { return text; }
};

class GMODULE Glyph::ControlInput : public Widget {};

class GMODULE Glyph::ColorPicker : public Widget {
	float color[4];
	void render(const double &deltaTime);

public:
	ColorPicker();
	void setColor(Color);
	Color getColor() const;
};

class GMODULE Glyph::ProgressBar : public Widget {
	float value = 0.f, maxValue = 100.f;

	void render(const double &deltaTime);

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

	void render(const double &deltaTime);

public:
	Histogram();
	~Histogram();
	void pushValue(float newValue);
};

#endif // !__widget