#pragma once
#ifndef __widget
#define __widget
#include <Gumball/Event.hpp>

enum class EVisibility {
	visible, hidden
};
enum class ELayout {
	vertical, horizontal
};

class GMODULE Widget {
	friend class WidgetContainer;

	WidgetContainer *container = nullptr;
	EVisibility visibility = EVisibility::hidden;

	Vector2 size;
	string label = "widget";	

public:
	virtual void render(const double &deltaTime) {}

	void setLabel(string newLabel) { this->label = newLabel; }
	void setVisibility(EVisibility newVisibility) { visibility = newVisibility; }
	void setSize(Vector2 newSize) { size = newSize; }

	Inline const string& getLabel() const { return label; }
	Inline EVisibility getVisibility() const { return visibility; }
	Inline Vector2 getSize() const { return size; }
	Inline WidgetContainer *getContainer() const { return container; }
};
typedef std::list<Widget *> Widgets;

class GMODULE WidgetContainer : public Widget {
private:
	std::list<Widget *> items;

protected:
	virtual void beginDraw();
	virtual void render(const double &deltaTime);
	virtual void endDraw();

public:
	
	WidgetContainer &operator<<(Widget *item);
	WidgetContainer &operator>>(Widget *item);
	WidgetContainer &operator<<(std::list<Widget *> items);
	WidgetContainer &operator>>(std::list<Widget *> items);

	Widget *operator[](const string &label);
	Inline bool hasChild(Widget *item) const { return std::find(items.begin(), items.end(), item) != std::end(items); };
};

class GMODULE UserWidget : public WidgetContainer {
public:
	UserWidget();
	void show();
	void hide();
};



namespace Clay {
	class GMODULE Text : public Widget {
		string text = "Text";

		void render(const double &deltaTime);

	public:
		void setText(string newText);
		string getText() { return text; }
		

		Dispatcher<Widget *, string, string> onTextUpdated; //obj, old text, new text;
	};

	class GMODULE InputText : public Widget {
		char inBuffer[256] = {};
		string text;

		void render(const double &deltaTime);

	public:
		string getText() const { return text; }

		Dispatcher<Widget *, string, string> onTextUpdated; //obj, old text, new text;
	};

	class GMODULE Button : public Widget {
		bool state = false;

		void render(const double &deltaTime);

	public:
		Inline bool getState() const { return state; }
		Dispatcher<Widget *, bool> onClick; //obj, newState
	};

	class GMODULE ProgressBar : public Widget {
		float value = 0.f, maxValue = 100.f;
	
		void render(const double &deltaTime);

	public:
		void setValue(float newValue);
		void setMaxValue(float newValue);
		float getValue() const { return value; }
		float getMaxValue() const { return maxValue; }
		float getFraction() const { return value / maxValue; }
		bool atMax() const { return value == maxValue; }
	};

	class GMODULE Histogram : public Widget {
		int valuesCount = 20;
		float *values;

		void render(const double &deltaTime);
	
	public:
		Histogram();
		~Histogram();

		void pushValue(float newValue);
	};
};


#endif // !__widget