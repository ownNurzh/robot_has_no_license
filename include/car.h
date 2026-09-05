#pragma once

#include <array>


#include "raylib.h"
#include "util.h"
#include "game_datas.h"

class Car {
public:
    Vector2 position;
    Vector2 size;

    float acceleration {100.0f};
    float speed {0.0f};
    float turn_speed {100.0f};
    float steering_angle {0.0f};
    float angle {0.0f};

    float max_speed {200.0f};

    bool alive {true};
    Car(float x,float y,float w,float h);
    std::array<Collision,COLLISION_COUNT> get_collisions() const;
    void update(float delta_time);
    void gas(float hw,float delta_time);
    void turn(float hw,float delta_time);
    static void draw_car(const Car &car,bool dev = false);
};