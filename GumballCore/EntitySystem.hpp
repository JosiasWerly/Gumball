#pragma once
#ifndef _entitysystem
#define _entitysystem

#include <iostream>
#include <chrono>
#include <list>
#include <queue>
#include <array>
#include <unordered_map>
using namespace std;

//constexpr size_t EntSize = 2000000;
//struct Position {
//    float x, y;
//};
//struct Think {
//    char dt;
//};
//typedef int Entity;
//typedef const char *Name;
//
//class IComponentHolder {
//public:
//};
//
//template<class T>
//class TComponentHolder :
//    public IComponentHolder {
//public:
//    unordered_map<Entity, T> instances;
//};
//
//class ComponentManager {
//    unordered_map<Name, IComponentHolder *> components;
//public:
//    template<class t>
//    TComponentHolder<t> *get() {
//        Name name = typeid(t).raw_name();
//
//        TComponentHolder<t> *out = nullptr;
//        auto it = components.find(name);
//        if (it != components.end()) {
//            out = static_cast<TComponentHolder<t>*>(it->second);
//        }
//        else {
//            out = new TComponentHolder<t>;
//            components.insert({ name, out });
//        }
//        return out;
//    }
//
//    template<class t>
//    t &addComponent(Entity &ent) {
//        TComponentHolder<t> *compHolder = get<t>();
//        compHolder->instances.emplace(ent, t());
//        return compHolder->instances[ent];
//    }
//};
//
//class EntityManager {
//public:
//    list<Entity> available;
//    deque<Entity> entities;
//
//    Entity &newEntity() {
//        if (available.empty()) {
//            return entities.emplace_back(entities.size());
//        }
//        else {
//            Entity &out = entities.emplace_back(available.front());
//            available.pop_front();
//            return out;
//        }
//    }
//};
//
//template<class T>
//struct View {
//private:
//    TComponentHolder<T> &target;
//public:
//    View(TComponentHolder<T> &target) :
//        target(target) {
//    }
//    void each(void(*fn)(T &)) {
//        for (auto &t : target.instances) {
//            fn(t.second);
//        }
//    }
//};
//
//class Ant {
//    EntityManager entMan;
//    ComponentManager compMan;
//public:
//    Entity &create() {
//        return entMan.newEntity();
//    }
//
//    template<class t>
//    t &addComponent(Entity &ent) {
//        return compMan.addComponent<t>(ent);
//    }
//
//    template<class t>
//    View<t> view() {
//        return View<t>(*compMan.get<t>());
//    }
//};
//
//void SysPos(Ant &ant) {
//    auto vw = ant.view<Position>();
//    vw.each(
//        [](Position &pos) {
//            pos.x += 1;
//            pos.y += 1;
//        }
//    );
//}
//void SysThink(Ant &ant) {
//    auto vw = ant.view<Think>();
//    vw.each(
//        [](Think &t) {
//            t.dt = rand() % 120 + 127;
//        }
//    );
//}
//void Test() {
//    Ant ant;
//    for (size_t i = 0; i < EntSize; i++) {
//        auto ent = ant.create();
//        Position &p = ant.addComponent<Position>(ent);
//        p.y = p.x = i;
//
//        Think &t = ant.addComponent<Think>(ent);
//        t.dt = rand() % 127 + 127;
//    }
//    SysPos(ant);
//    SysThink(ant);
//}




//typedef const char *ComponentName;
//struct Entity {
//    int id;
//    unordered_map<ComponentName, int> components;
//};
//
//class IComponentPool {
//public:
//    IComponentPool() = default;
//    virtual ~IComponentPool() = default;
//};
//
//template<class T>
//class TComponentPool : 
//    public IComponentPool{
//    friend class EntitySystem;
//    string name;
//    deque<int> availableIds;
//    unordered_map<int, T> instances;
//public:    
//    TComponentPool() = default;
//    ~TComponentPool() = default;
//
//    int alloc(T &&init = T()) {
//        if (!availableIds.empty()) {
//            instances.insert({ availableIds.back(), init });
//            availableIds.pop_back();
//        }
//        else {
//            instances.insert({ instances.size(), init });
//        }
//        return instances.end()->first;
//    }
//    void dealloc(int id) {
//        if (instances.contains(id)) {
//            instances.erase(id);
//            availableIds.push_back(id);
//        }
//    }
//    T& allocT(T &&init = T()) {
//        alloc(init);
//        return (instances.end())->second;
//    }
//    void deallocT(T &trg) {
//        int id = findId(trg);
//        if (id != -1) {
//            dealloc(id);
//        }
//    }
//
//    T &find(int id) {
//        return instances[id];
//    }
//    int findId(T &trg) {
//        for (auto i : instances) {
//            if (i.second == trg) {
//                return i.first;
//            }
//        }
//        return -1;
//    }
//    unordered_map<int, T> &getInstances() { return instances; }
//    ComponentName getName() const { return name.c_str(); }
//};
//
//class EntitySystem {
//    unordered_map<ComponentName, IComponentPool *> pools;
//    TComponentPool<Entity> entities;
//public:
//    EntitySystem() = default;
//    virtual ~EntitySystem() = default;
//
//    template<class t> 
//    TComponentPool<t> *getPool() {
//        ComponentName name = typeid(t).raw_name();
//        TComponentPool<t> *out = nullptr;
//
//        auto it = pools.find(name);
//        if (it != pools.end()) {
//            out = static_cast<TComponentPool<t>*>(it->second);
//        }
//        return out;
//    }
//    template<class t>
//    TComponentPool<t> *addPool() {
//        ComponentName name = typeid(t).raw_name();
//        TComponentPool<t> *out = new TComponentPool<t>();
//
//        pools.insert({ name, out });
//        out->name = name;
//        return out;
//    }
//
//    void newEntity(Entity &e) {
//        e.id = entities.alloc();
//    }
//    void delEntity(Entity &e) {
//        entities.dealloc(e.id);
//    }
//
//    template<class t>
//    void addComp(Entity &e, t *trg = nullptr) {
//        auto p = getPool<t>();
//        if (trg) {
//            p->findId(*trg);
//        }
//        e.components.insert({ p->getName(), p->alloc()});
//    }
//    template<class t>
//    void delComp(Entity &e, t *trg = nullptr) {
//        auto p = getPool<t>();
//        p->dealloc(e.components[p->getName()]);
//        e.components.erase(p->getName());
//    }
//};

#endif // !_entitysystem5