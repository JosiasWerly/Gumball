#ifndef _widgetoverlay
#define _widgetoverlay
#include <Render/RenderOverlay.hpp>

class WidgetOverlay;
class Widget;

class GMODULE WidgetOverlay : public RenderOverlay {
    friend class Widget;

    struct ImGuiIO *guiIO = nullptr;
    std::list<Widget *> elements;

public:
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
	string name() override { return "widget"; }

    WidgetOverlay &operator<<(Widget *element);
    WidgetOverlay &operator>>(Widget *element);
};


//TODO: add a default name to IWidget::Name, since every entity needs a name
//TODO: implement an event based, we're handling by pooling everything...
class GMODULE Widget {
	friend class WidgetOverlay;
	friend class WidgetContainer;

	bool isVisible = true;
	WidgetContainer *parent = nullptr;
	string name = "widget";

public:
    virtual void render(const double &deltaTime);
	virtual void setVisibility(bool newVisible);
	Inline bool getVisibility() const { return isVisible; }
	
	void setName(string name) { this->name = name; }
	string getName() { return name; }

    Inline WidgetContainer *getParent() const { return parent; }
};

class GMODULE WidgetContainer : public Widget {
	list<Widget *> children;
public:
	WidgetContainer() = default;
	
	virtual void render(const double &deltaTime) override;
	virtual void beginDraw();
	virtual void endDraw();

	WidgetContainer &operator<<(Widget *child);
	WidgetContainer &operator>>(Widget *child);
	void addChildren(list<Widget *> children);
	void delChildren(list<Widget *> children);
	Inline bool hasChild(Widget *child) const { return std::find(children.begin(), children.end(), child) != std::end(children); };
};

class GMODULE UserWidget : public WidgetContainer {

public:
	UserWidget();
	void show();
	void hide();
	
	//virtual void render(const double &deltaTime) {}
	//virtual void beginDraw() {}
	//virtual void endDraw() {}
};

namespace UI {
	class GMODULE Text : public Widget {
	public:
		string text = "Text";

		Text() = default;
		void render(const double &deltaTime);
	};

	class GMODULE InputText : public Widget {
		char inBuffer[256] = {};
		string str;

	public:
		void render(const double &deltaTime);
		bool getInput(string &out) const;
	};

	class GMODULE Button : public Widget {
		bool clicked = false;

	public:
		string text = "Button";

		Button() = default;
		void render(const double &deltaTime);
		Inline bool isClicked() { return clicked; }
	};
};

#endif // !_widgetoverlay