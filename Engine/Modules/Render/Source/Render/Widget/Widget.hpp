#pragma once
#ifndef __widget
#define __widget
#include "WidgetOverlay.hpp"

struct ImGuiInputTextCallbackData;
class Widget;
class UserWidget;

namespace Glyph {
	class Container;
};

typedef std::list<Widget*> Widgets;

enum class eVisibility {
	visible, hidden
};

enum class eLayout {
	vertical, horizontal
};

template<class T> using WidgetProp = TProperty<Widget, T>;
template<class T> using WidgetPropArr = TPropertyArr<Widget, T>;

class GMODULE Widget {
	friend class WidgetContainer;
	friend class UserWidget;
	friend class Glyph::Container;

protected:
	virtual void render() = 0;

public:
	WidgetProp<string> label;

	Widget();
	virtual ~Widget();
};

class GMODULE UserWidget : public UIDrawable, public Widget {
	void onShowEvent(const Widget *, const eVisibility &, const eVisibility &);
	void render();

public:
	WidgetProp<Vector2> size;
	WidgetProp<eVisibility> visibility;
	WidgetPropArr<Widget> container;

	UserWidget();
	~UserWidget();
};
#endif // !__widget