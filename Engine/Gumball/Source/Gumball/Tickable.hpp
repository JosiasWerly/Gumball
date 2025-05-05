#ifndef __tickable
#define __tickable


class Tick {
public:
	virtual ~Tick() = default;
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);
};

#endif // __tickable
