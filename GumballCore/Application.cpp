#include "Engine.hpp"

#include <iostream>
using namespace std;


namespace x {
	class Container {
	protected:
		Container(){}
	public:
		void *vRaw = nullptr;
		unsigned refCounter = 1;

		virtual ~Container() {
		}
	};

	template<class T>
	class TContainer : 
		public Container{
	public:
		T **raw = nullptr;
		TContainer(T *ptr) : 
			raw (reinterpret_cast<T**>(&vRaw)) {
			*raw = ptr;
		}
		virtual ~TContainer() {
			delete *raw;
		}
	};


	class Var {
	public:
		Container *container = nullptr;

		inline void release() {
			if (--container->refCounter == 0) {
				delete container;
				container = nullptr;
			}
		}
		inline void changeRef(Container *newContainer) {
			release();
			container = newContainer;
			container->refCounter++;
		}
		

		template<class t> 
		Var(t *ptr = nullptr) {
			container = new TContainer<t>(ptr);
		}
		Var(const Var &other) {
			container = other.container;
			++container->refCounter;
		}
		Var(Var &&other) {
			container = other.container;
			++container->refCounter;
		}
		virtual ~Var() {
			release();
		}
		Var &operator=(const Var &other) {
			changeRef(other.container);
			return *this;
		}


		template<class t>
		t *pin() {
			return reinterpret_cast<t *>(container->vRaw);
		}

		bool operator==(const Var &other) const {
			return container == other.container;
		}
		operator bool const() {
			return container->vRaw;
		}

		inline unsigned refCount() { return container->refCounter; }
	};

	template<class T>
	class TVar : 
		public Var{
	public:
	};

};


int main(int argc, char *argv[]) {
	x::Var v(new int(1234));

	//void *p = new Foo;
	//delete p;


	Engine::setInstance(new Engine);
	Engine::inst->args(argc, argv);
	Engine::inst->tick();
	return 0;
}