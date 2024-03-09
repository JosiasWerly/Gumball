#include "Activator.Core.hpp"
#include "Activator.hpp"

#include "WorldSystem.hpp"
#include "Actor.hpp"

ActivatorPackage_Core::ActivatorPackage_Core() {
	signatures = {
		addType(Actor), 
		addType(GameObject), 
		addType(ActorComponent),
		addType(CameraComponent),
		addType(MeshComponent),
	};
}