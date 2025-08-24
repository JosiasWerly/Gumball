#pragma once
#ifndef __dispatcher
#define __dispatcher
#include <list>
#include <functional>

namespace Dispatcher {
	enum class ePolicy { single, multi };

	template<typename ...TSignature> struct TBind;
	template<ePolicy, typename ...TSignature> class TSignal;
	template<typename ...TSignature> class TSignalProxy;

	template<class TRet, class ...TArgs>
	struct TBind<TRet(TArgs...)> {
		using TFunction = std::function<TRet(TArgs...)>;
		TFunction fn;

		template<class TFn>
		TBind(TFn &&fn) :
			fn(std::move(fn)) {
		}

		template<class TObj>
		TBind(TObj *obj, TRet(TObj:: *method)(TArgs...)) :
			fn(std::move([=](TArgs... args) { (obj->*method)(args...); })) {
		}

		TBind(const TBind &) = delete;
		TBind(TBind &&) = delete;
	};

	template<class TChild, class TRet, class ...TArgs>
	class TBase {
		inline TChild &child() { return static_cast<TChild &>(*this); }
	public:
		using TBind = TBind<TRet(TArgs...)>;
		using TFunction = std::function<TRet(TArgs...)>;

		TBase() = default;
		~TBase() = default;

		void bind(TBind &&bind) {
			child()._bind(std::move(bind.fn));
		}
		void unbind(TBind &&bind) {
			child()._unbind(std::move(bind.fn));
		}
		TRet invoke(TArgs... args) {
			return child()._invoke(args...);
		}
		TRet operator()(TArgs... args) {
			return child()._invoke(args...);
		}
		
		TRet invoke(TArgs... args) const {
			return child()._invoke(args...);
		}
		TRet operator()(TArgs... args) const {
			return child()._invoke(args...);
		}
		bool isBound() const {
			return child()._isBound();
		}
	};

	template<class TRet, class ...TArgs>
	class TSignal<ePolicy::single, TRet(TArgs...)> :
		public TBase<TSignal<ePolicy::single, TRet(TArgs...)>, TRet, TArgs...> {
		template<class T, class TRet, class ...TArgs> friend class TBase;

		using Base = TBase<TSignal<ePolicy::single, TRet(TArgs...)>, TRet, TArgs...>;
		using TFunction = Base::TFunction;

		TFunction target;

		void _bind(TFunction &&fn) { target = fn; }
		void _unbind(TFunction &&fn) { target = nullptr; }
		TRet _invoke(TArgs... args) {
			if (target)
				return target(args...);
			return TRet();
		}
		bool _isBound() { return static_cast<bool>(target); }

	public:
		TSignal() = default;
		~TSignal() = default;
	};

	template<class ...TArgs>
	class TSignal<ePolicy::multi, void(TArgs...)> :
		public TBase<TSignal<ePolicy::multi, void(TArgs...)>, void, TArgs...> {
		template<class T, class TRet, class ...TArgs> friend class TBase;

		using Base = TBase<TSignal<ePolicy::multi, void(TArgs...)>, void, TArgs...>;
		using TFunction = Base::TFunction;

		std::list<TFunction> targets;

		void _bind(TFunction &&fn) { targets.emplace_back(fn); }
		void _unbind(TFunction &&fn) { targets.remove(fn); }
		void _invoke(TArgs... args) {
			for (auto &t : targets)
				t(args...);
		}
		bool _isBound() { return !targets.empty(); }

	public:
		TSignal() = default;
		~TSignal() = default;
	};
};

template<class TRet, class ...TArgs>
using Signal = Dispatcher::TSignal<Dispatcher::ePolicy::single, TRet, TArgs...>;

template<class ...TArgs>
using Signals = Dispatcher::TSignal<Dispatcher::ePolicy::multi, TArgs...>;
#endif // !__dispatcher