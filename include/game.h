#pragma once

#include "raylib.h"

class Game {
public:
    void draw_city();
    void start();
    Vector2 get_random_spawn();
};