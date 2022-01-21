#include "Engine.hpp"
#include "AssetManager.hpp"

#include "Shaders.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Math.hpp"
#include "RenderSystem.hpp"
#include "Event.hpp"

using namespace std;


static void printVector(Vector3 v) {
	cout << "x:" << v.x << " y:" << v.y << " z:" << v.z << endl;
}

int main() {
	auto &engine = Engine::instance();
	engine.initialize();

	auto &renderSystem = RenderSystem::instance();
	auto &assetSystem = AssetsSystem::instance();
	auto &inputSystem = InputSystem::instance();
	assetSystem.loadAllFiles("res\\");
	
	View v;
	v.viewMode.setProjectionPerspective();
	v.transform.position.z = -10;
	renderSystem.pushView(0, &v);

	DrawInstance dd;
	dd.setMesh("cube");
	
	renderSystem.pushDrawInstance(0, &dd);


	engine.onPlay();

	const float vel = 0.01f;
	while (1) {	
		renderSystem.tick();
		inputSystem.tick();


		if (inputSystem.isKeyDown(Input::EKeyCode::W))
			dd.transform.position += dd.transform.rotator.forward() * vel;
		else if (inputSystem.isKeyDown(Input::EKeyCode::S))
			dd.transform.position -= dd.transform.rotator.forward() * vel;

		if (inputSystem.isKeyDown(Input::EKeyCode::D))
			dd.transform.position += dd.transform.rotator.right() * vel;
		else if (inputSystem.isKeyDown(Input::EKeyCode::A))
			dd.transform.position -= dd.transform.rotator.right() * vel;

		if (inputSystem.isKeyDown(Input::EKeyCode::UP))
			dd.transform.rotator.rotate(1, 0, 0);
		else if (inputSystem.isKeyDown(Input::EKeyCode::DOWN))
			dd.transform.rotator.rotate(-1, 0, 0);

		if (inputSystem.isKeyDown(Input::EKeyCode::LEFT))
			dd.transform.rotator.rotate(0, 0, -1);
		else if (inputSystem.isKeyDown(Input::EKeyCode::RIGHT))
			dd.transform.rotator.rotate(0, 0, 1);
	}
	engine.shutdown();
	return 0;
}