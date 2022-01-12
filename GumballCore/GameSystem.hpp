#pragma once
#ifndef _gamesystem
#define _gamesystem

#include <iostream>
#include <list>
using namespace std;

#include "EngineSystem.hpp"
#include "Patterns.hpp"

//class GameObject {
//public:
//    string name;
//    GameObject();
//    virtual ~GameObject();
//
//    virtual void begingPlay() = 0;
//    virtual void endPlay() = 0;
//    virtual void tick() = 0;
//};
//
//class GameSystem :
//    public IEngineSystem,
//    public Singleton<GameSystem> {
//
//    template <typename T> 
//    friend Var<T> instantiate(T* gameObject);
//    friend void destroy(GameObject*& gameObject);
//
//
//    list<Var<GameObject>> toBeginPlay, toEndPlay, gameObjects;
//public:
//    void shutdown() override;
//    void tick() override;
//
//};
//
//template<class T>
//static Var<T>& instantiate(T* gameObject = new T) {
//    auto& gs = GameSystem::instance();
//    gs.drawinstances.emplace_back();
//    return gs.drawinstances.back();
//}
//static void destroy(Var<GameObject>& gameObject) {
//    delete *gameObject;
//}


#endif // !_gameobject


