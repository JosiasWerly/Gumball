#include <GumballCore/Definitions.hpp>
#include <GumballCore/ProjectLinker.hpp>
//#include <GumballCore/Activator.hpp>

//struct ActivatorPackage_Project : public ActivatorPackage {
//	ActivatorPackage_Project();
//};


class MyProject : public Project {
public:	
	virtual void attached();
	virtual void detached();
};

Extern GBPROJECT void *EntryPoint();
