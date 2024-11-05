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

typedef std::list<Widget *> Widgets;

enum class eVisibility {
	visible, hidden
};

enum class eLayout {
	vertical, horizontal
};

template<class T>
struct TProp {
private:
	const Widget *owner;
	T value;

public:
	Delegate<void(const Widget *obj, const T &oldValue, const T &newValue)> notify;

	explicit TProp(Widget *owner) : owner(owner) {}
	explicit TProp(Widget *owner, const T &&value) : owner(owner), value(value) {}
	TProp(const TProp&) = delete;

	Inline void operator()(T value) {
		if (this->value == value)
			return;
		T oldValue = this->value;
		this->value = value;
		notify.invoke(owner, oldValue, this->value);
	}
	Inline const T &operator()() const { return value; }
	Inline T &operator()() { return value; }
};

template<>
struct TProp<Widgets> {
private:
	const Widget *owner;
	Widgets widgets;

public:
	TProp(Widget *owner);
	~TProp();
	TProp &operator<<(Widget *widget);
	TProp &operator>>(Widget *widget);
	TProp &operator<<(Widgets widgets);
	TProp &operator>>(Widgets widgets);
	const Widget *operator[](const string &label);

	Inline const Widget *getOwner() const { return owner; }
	Inline const Widgets &getWidgets() const { return widgets; };
	Inline bool hasChild(Widget *item) const { return std::find(widgets.begin(), widgets.end(), item) != std::end(widgets); };
};

class GMODULE Widget {
	friend class WidgetContainer;
	friend class UserWidget;
	friend class Glyph::Container;

protected:
	virtual void render() = 0;

public:
	TProp<string> label;

	Widget();
	virtual ~Widget();
};

class GMODULE UserWidget : public UIDrawable, public Widget {
	void onShowEvent(const Widget *, const eVisibility &, const eVisibility &);
	void render();

public:
	TProp<Vector2> size;
	TProp<Widgets> container;
	TProp<eVisibility> visibility;

	UserWidget();
	~UserWidget();
};

inline TProp<Widgets>::TProp(Widget *owner) :
	owner(owner) {
}
inline TProp<Widgets>::~TProp() {
	for (auto w : widgets)
		delete w;
}
inline TProp<Widgets> &TProp<Widgets>::operator<<(Widget *widget) {
	//if (owner->container)
	//	widget->container->operator>>(widget);
	widgets.push_back(widget);
	//widget->container = this;
	return *this;
}
inline TProp<Widgets> &TProp<Widgets>::operator>>(Widget *widget) {
	//if (this != widget->container)
	//	return *this;
	//
	widgets.remove(widget);
	//widget->container = nullptr;
	return *this;
}
inline TProp<Widgets> &TProp<Widgets>::operator<<(Widgets widgets) {
	for (auto w : widgets)
		(*this) << w;
	return *this;
}
inline TProp<Widgets> &TProp<Widgets>::operator>>(Widgets widgets) {
	for (auto w : widgets)
		(*this) >> w;
	return *this;
}
inline const Widget *TProp<Widgets>::operator[](const string &label) {
	for (auto &i : widgets) {
		if (i->label() == label)
			return i;
	}
	return nullptr;
}

#endif // !__widget