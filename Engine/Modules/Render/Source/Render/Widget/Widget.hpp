#pragma once
#ifndef __widget
#define __widget
#include "WidgetOverlay.hpp"

struct ImGuiInputTextCallbackData;
class Widget;
class UserWidget;

namespace Glyph {
	class Container;
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
	void *operator new(size_t size);
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

namespace Glyph {
	class GMODULE Container : public Widget {
	protected:
		void render();

	public:
		TProp<eLayout> layout;
		TProp<Widgets> container;
		Container();
	};

	class GMODULE Button : public Widget {
		void render();

	public:
		Delegate<void(Widget *obj)> onClick;
		Button() = default;
	};

	class GMODULE CheckBox : public Widget {
		void render();

	public:
		TProp<bool> state;
		CheckBox();
	};

	class GMODULE Combo : public Widget {
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

	class GMODULE Text : public Widget {
		void render();

	public:
		TProp<string> text;
		Text();
	};

	class GMODULE TextInput : public Widget {
		friend int callback(ImGuiInputTextCallbackData *data);
		static int callback(ImGuiInputTextCallbackData *data);
		void render();

	public:
		TProp<string> text;
		TProp<bool> multiline;
		TProp<Vector2> size;
		TextInput();
	};

	class GMODULE ColorPicker : public Widget {
		float color[4];
		void render();

	public:
		ColorPicker();
		void setColor(Color);
		Color getColor() const;
	};

	class GMODULE ProgressBar : public Widget {
		float value = 0.f, maxValue = 100.f;

		void render();

	public:
		TProp<Vector2> size;

		ProgressBar();
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

		void render();

	public:
		TProp<Vector2> size;

		Histogram();
		~Histogram();
		void pushValue(float newValue);
	};

	class GMODULE IntergerInput : public Widget {
		int *pValues = nullptr;
		unsigned pValuesSize = 0;

		void render();

	public:
		IntergerInput(unsigned size);
		~IntergerInput();
	};

	class GMODULE FloatInput : public Widget {
		float *pValues = nullptr;
		unsigned pValuesSize = 0;

		void render();

	public:
		FloatInput(unsigned size);
		~FloatInput();
	};
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