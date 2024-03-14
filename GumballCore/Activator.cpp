#include "Activator.hpp"
#include "picojson.h"
using namespace std;


Class Property::asClass() {
	return is<Class>() ? Class(address, dynamic_cast<FieldObject *>(field)->getClass()) : Class();
}
SerialStream Property::toStream() {
	return is<Class>() ? asClass().toStream() : field->toStream(address);
}
void Property::fromStream(SerialStream stream) {
	return is<Class>() ? asClass().fromStream(stream) : field->fromStream(address, stream);
}

void Activator::add(FieldClass *object) {
	if (!object)
		return;

	if (objects.contains(object->getName()))
		return;

	objects[object->getName()] = object;
}
void Activator::del(const string &name) {
	if (!objects.contains(name))
		return;

	delete objects[name];
	objects.erase(name);
}
FieldClass *Activator::get(const string &name) {
	return objects.contains(name) ? objects[name] : nullptr;
}

Class ImplClass::getClass(){
	return Class(reinterpret_cast<intptr_t>(this), Activator::instance()->get(getClassName()));
}