#pragma once
#ifndef __dispatcher
#define __dispatcher
#include <list>
#include <functional>

namespace Dispatcher {
	enum class ePolicy : char { single, multi };

	template<ePolicy, typename ...TSignature> class Delegate;

	template<class TChild, class TRet, class ...TArgs>
	class TDelegate {
		inline TChild &child() { return static_cast<TChild &>(*this); }
		inline const TChild &child() const { return static_cast<const TChild &>(*this); }
	public:
		using TFunction = std::function<TRet(TArgs...)>;

		TDelegate() = default;
		~TDelegate() = default;

		void Bind(auto fn) { child()._bind(std::move(fn)); }
		void Bind(auto *obj, auto method) { child()._bind(std::move([obj, method](TArgs... args)->TRet { return (obj->*method)(std::forward<TArgs>(args)...); })); }
		void Unbind() { child()._unbind(); }
		TRet Invoke(TArgs... args) { return child()._invoke(args...); }
		TRet Invoke(TArgs... args) const { return child()._invoke(args...); }
		bool IsBound() const { return child()._isBound(); }
		TRet operator()(TArgs... args) { return child()._invoke(args...); }
		TRet operator()(TArgs... args) const { return child()._invoke(args...); }
		operator bool() const { return IsBound(); }
	};

	template<class TRet, class ...TArgs>
	class Delegate<ePolicy::single, TRet(TArgs...)> :
		public TDelegate<Delegate<ePolicy::single, TRet(TArgs...)>, TRet, TArgs...> {
		template<class T, class TRet, class ...TArgs> friend class TDelegate;

		using Base = TDelegate<Delegate<ePolicy::single, TRet(TArgs...)>, TRet, TArgs...>;
		using TFunction = Base::TFunction;

		TFunction target;

		void _bind(TFunction &&fn) { target = fn; }
		void _unbind(TFunction &&fn) { target = nullptr; }
		TRet _invoke(TArgs... args) const {
			if (target)
				return target(args...);
			return TRet();
		}
		bool _isBound() const { return static_cast<bool>(target); }

	public:
		Delegate() = default;
		~Delegate() = default;
	};

	template<class ...TArgs>
	class Delegate<ePolicy::multi, void(TArgs...)> :
		public TDelegate<Delegate<ePolicy::multi, void(TArgs...)>, void, TArgs...> {
		template<class T, class TRet, class ...TArgs> friend class TDelegate;

		using Base = TDelegate<Delegate<ePolicy::multi, void(TArgs...)>, void, TArgs...>;
		using TFunction = Base::TFunction;

		std::list<TFunction> targets;

		void _bind(TFunction &&fn) { targets.emplace_back(fn); }
		void _unbind(TFunction &&fn) { targets.clear(); }
		void _invoke(TArgs... args) const {
			for (auto &t : targets)
				t(args...);
		}
		bool _isBound() { return !targets.empty(); }

	public:
		Delegate() = default;
		~Delegate() = default;
	};
};

template<class TRet, class ...TArgs>
using Delegate = Dispatcher::Delegate<Dispatcher::ePolicy::single, TRet, TArgs...>;

template<class ...TArgs>
using Delegates = Dispatcher::Delegate<Dispatcher::ePolicy::multi, TArgs...>;
#endif // !__dispatcher