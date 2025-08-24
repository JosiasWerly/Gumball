#pragma once
#ifndef __glyph
#define __glyph
#include "WidgetOverlay.hpp"
#include "Widget.hpp"

namespace Glyph {
	class GMODULE Container : public Widget {
	protected:
		void render();

	public:
		WidgetProp<eLayout> layout;
		WidgetPropArr<Widget> container;
		Container();
	};

	class GMODULE Button : public Widget {
		void render();

	public:
		Signals<void(Widget *obj)> onClick;
		Button() = default;
	};

	class GMODULE CheckBox : public Widget {
		void render();

	public:
		WidgetProp<bool> state;
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
		WidgetProp<string> text;
		Text();
	};

	class GMODULE TextInput : public Widget {
		friend int callback(ImGuiInputTextCallbackData *data);
		static int callback(ImGuiInputTextCallbackData *data);
		void render();

	public:
		WidgetProp<string> text;
		WidgetProp<bool> multiline;
		WidgetProp<Vector2> size;
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
		WidgetProp<Vector2> size;

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
		WidgetProp<Vector2> size;

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

#endif // !__glyph