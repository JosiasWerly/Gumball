#include "Gumball.hpp"

void processInput(GLFWwindow* window);


struct Mem{
	void* ptr = 0;
	unsigned int size = 0;
	string TypeOf = "";
};
static std::vector<Mem> memoryTable;
void peekMemory() {
	for (auto m : memoryTable) {
		cout << m.ptr << " " << m.size << " " << m.TypeOf << endl;
	}
}

template<class T> struct Dt {
public:
	T* value = nullptr;
	unsigned int refs = 1;
	Dt(T* init) : value(init){
		memoryTable.push_back(
			{
				this,
				sizeof(*this),
				" "
			}
		);
	}
	~Dt() {
		for (size_t i = 0; i < memoryTable.size(); i++)
			if (memoryTable[i].ptr == this)
				memoryTable.erase(memoryTable.begin()+i);
		delete value;
	}
};
template<class T> class Var {
public:
	typedef Var<T> varT;
	typedef Dt<T> dtT;

	dtT* dt;
	Var(T* ptr = nullptr) {
		dt = new dtT(ptr ? ptr : new T);
	}
	Var(varT& Other) {
		dt = Other.dt;
		dt->refs++;
	}
	~Var() {
		if (dt && --dt->refs == 0) {
			delete dt;
		}
	}

	varT& operator=(varT &cpy) {
		if (dt != cpy.dt) {
			if (dt)
				dt->refs--;
			dt = cpy.dt;
			dt->refs++;
		}
		return *this;
	}

	operator T* () {
		return dt ? dt->value : nullptr;
	}
	T* operator=(T* newValue) {
		if (dt->value)
			delete dt->value;
		dt->value = newValue;
		return dt->value;
	}
	T* operator()() {
		return dt->value;
	}

	operator bool() {
		return dt->value;
	}
};






int main() {
	Var<int> a, b, c;
	a = new int(20);
	b = a;
	a = b;
	peekMemory();
	c = new int(30);
	c = a;
	peekMemory();

	window.create("Gumball", 800, 600);
	assetManager.loadAssets("res/");

	Camera* mainCamera = render.camera = new Camera();
	mainCamera->transform.position += glm::vec3(0, 0, -10);

	constexpr int recSize = 2;
	vector<Meshdata*> drawObjects;
	for (size_t i = 0; i < recSize; i++) {
		Meshdata* newMesh = new Meshdata;
		if (i % 2) {
			newMesh->loadMesh("plane");
			newMesh->transform.position += glm::vec3(0, 2, 0);
			newMesh->transform.rotation.rotate(90, 0, 0);
		}
		else {
			newMesh->loadMesh("cube");
			newMesh->transform.position += glm::vec3(0, -2, 0);
		}
		drawObjects.push_back(newMesh);
		render << newMesh;
	}
	float turnRate = 4;


	Transform* t = &mainCamera->transform;
	while (true) {
		if (t) {
			if (engine.keyPressed(GLFW_KEY_W))
				t->rotation.rotate(turnRate, 0, 0);
			else if (engine.keyPressed(GLFW_KEY_S))
				t->rotation.rotate(-turnRate, 0, 0);

			if (engine.keyPressed(GLFW_KEY_Q))
				t->rotation.rotate(0, turnRate, 0);
			else if (engine.keyPressed(GLFW_KEY_E))
				t->rotation.rotate(0, -turnRate, 0);

			if (engine.keyPressed(GLFW_KEY_A))
				t->rotation.rotate(0, 0, turnRate);
			else if (engine.keyPressed(GLFW_KEY_D))
				t->rotation.rotate(0, 0, -turnRate);
		}
		window.disposeRender();
	}
	return 0;
}

/*
##### Gumball 0.1 #####
     refator:
[mk1] uniforms system
[mk1] texture && UV
[TODO] input wrapper
[todo] separe gamethread vs drawThread && querrySystem
[todo] collision detection
[todo] traces, lines
*/


