#include "Project.hpp"

#include "Source.hpp"

void MyProject::attached() {
	cout << "loaded" << endl;
	//Activator::instance()->add("project", ActivatorPackage_Project());


	srand(0);
	auto cameraActor = new Actor;
	{
		auto cameraComp = new CameraComponent;
		cameraActor->addComponent(cameraComp);
		cameraActor->transform.position.z = -20;
	}

	for (size_t i = 0; i < 5; i++) {
		new MyActor;
	}
}
void MyProject::detached() {
	cout << "unloaded" << endl;
	//Activator::instance()->del("project");
}

Extern GBPROJECT void *EntryPoint() { 
	return new MyProject;
}