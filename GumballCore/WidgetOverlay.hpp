#ifndef _widgetoverlay
#define _widgetoverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WidgetOverlay;
class Widget;

class WidgetOverlay :
    public IRenderOverlay {
    friend class Widget;

    ImGuiIO *guiIO = nullptr;
    std::list<Widget *> elements;
public:

    WidgetOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;

    WidgetOverlay &operator<<(Widget *element);
    WidgetOverlay &operator>>(Widget *element);
};


//TODO: add a default name to IWidget::Name, since every entity needs a name
//TODO: implement an event based, we're handling by pooling everything...
class Widget {
	friend class WidgetOverlay;
	friend class WidgetContainer;

	bool isVisible = true;
	WidgetContainer *parent = nullptr;

public:
    virtual void render(const double &deltaTime);
	void setVisibility(bool newVisible);
	
	Inline bool getVisibility() const { return isVisible; }
    Inline WidgetContainer *getParent() const { return parent; }
};

class WidgetContainer : public Widget {
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

class UserWidget : public WidgetContainer {
public:
	UserWidget() = default;
	//possible methods to implement on child of this class
	//virtual void render(const double &deltaTime) {}
	//virtual void beginDraw() {}
	//virtual void endDraw() {}
};

namespace UI {
	class Text : public Widget {
	public:
		string text = "Text";

		Text() = default;
		void render(const double &deltaTime);
	};

	class InputText : public Widget {
		char inBuffer[256] = {};
		string str;

	public:
		void render(const double &deltaTime);
		bool getInput(string &out) const;
	};

	class Button : public Widget {
		bool clicked = false;

	public:
		string text = "Button";

		Button() = default;
		void render(const double &deltaTime);
		Inline bool isClicked() { return clicked; }
	};
};

#endif // !_widgetoverlay