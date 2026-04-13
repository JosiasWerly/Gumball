#include <Gumball/Plugin/Project.hpp>

class GGAME MyProject : public Plugin::Project {
public:
	void Attached();
	void Detached();
};

Extern GGAME void *EntryPoint() { return new MyProject; }