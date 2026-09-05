#pragma once

#include <vector>

#include "raylib.h"
#include "car.h"

class Game {
public:
    std::vector<Car> cars;
    Vector2 *goal_position {};
    // Game();
    // ~Game();
    void draw_city();
    void draw_cars();
    void start();
    void loop();
    Vector2 get_random_spawn();
    void add_car(float x,float y);
};