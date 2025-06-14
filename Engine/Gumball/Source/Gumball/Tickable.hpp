#ifndef __tickable
#define __tickable

#include <list>
#include <deque>
#include <unordered_map>


#include <unordered_map>
#include <list>
#include <optional>
#include <stdexcept>
#include <deque>


//class GENGINE Tick {
//public:
//	virtual void beginPlay() {}
//	virtual void endPlay() {}
//	virtual void tick(const double &deltaTime) {}
//	virtual TickGraph &tickGraph() = 0; //maybe this has to be another object
//	
//	void setTick(float tick) {}
//	Inline float getTick() const { return 0.f; }
//};
//
//class GENGINE TickGraph {
//public:
//	std::list<Tick *> tickables;
//
//	virtual ~TickGraph() = default;
//	virtual void beginPlay() {}
//	virtual void endPlay() {}
//	virtual void tick(const double &deltaTime) {}
//};
//
//
//class GENGINE TickController {
//	std::list<TickGraph> graphs;
//
//public:
//	void beginPlay();
//	void endPlay();
//	void tick(const double &deltaTime);
//};
#endif // __tickable
