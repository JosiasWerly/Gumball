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
	friend class WidgetOverlay;
	friend class WidgetContainer;

	WidgetContainer *container = nullptr;
	EVisibility visibility = EVisibility::hidden;

protected:
	string label = "widget";
	virtual void render(const double &deltaTime) {}

public:
	void setLabel(string newLabel) { this->label = newLabel; }
	void setVisibility(EVisibility newVisibility) { visibility = newVisibility; }
	Inline const string& getLabel() { return label; }
	Inline EVisibility getVisibility() const { return visibility; }
	Inline WidgetContainer *getContainer() const { return container; }
};

class GMODULE WidgetContainer : public Widget {
	list<Widget *> items;

protected:
	virtual void render(const double &deltaTime) override;
	virtual void beginDraw();
	virtual void endDraw();

public:
	ELayout layout = ELayout::vertical;

	WidgetContainer &operator<<(Widget *item);
	WidgetContainer &operator>>(Widget *item);
	void pushItems(list<Widget *> items);
	void popItems(list<Widget *> items);
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
	
	protected:
		void render(const double &deltaTime);

	public:
		void setText(string newText);
		string getText() { return text; }

		Dispatcher<Widget *, string, string> onTextUpdated; //obj, old text, new text;
	};

	class GMODULE InputText : public Widget {
		char inBuffer[256] = {};
		string text;
	
	protected:
		void render(const double &deltaTime);
	
	public:
		string getText() const { return text; }

		Dispatcher<Widget *, string, string> onTextUpdated; //obj, old text, new text;
	};

	class GMODULE Button : public Widget {
		bool state = false;

	protected:
		void render(const double &deltaTime);

	public:
		Inline bool getState() const { return state; }
		Dispatcher<Widget *, bool> onClick; //obj, newState
	};
};


#endif // !__widget