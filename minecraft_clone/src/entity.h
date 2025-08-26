#pragma once
#include <iostream>
// #include "renderer.h"

class Entity {

public:

    enum class Type { // no reason not to be a class
        // must be public because it needs to return a type
        ENEMY, 
        ITEM
        // add more
    };

    enum class Component {
        // bit set in the form of a uint64
        TEXTURE = 1 << 0,
        MOVE = 1 << 1
    };

    Type getType() const{
        return type;
    }

    std::string getName() const {
        return name;
    }

    bool hasComponent(Component c) const{
        if (componentList & (uint64_t)c) {
            return true;
        }
        return false;
    }

    // draw function

    // interact function

    // etc...

private:
    std::string name;
    uint64_t componentList;
    Type type;

};