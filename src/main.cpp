#include <ctime>

#include "game.h"

int main()
{
    SetRandomSeed(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Robot has no license");

    SetTargetFPS(60);
    Game *game = new Game();

    Vector2 random_pos = game->get_random_spawn();
    game->add_car(random_pos.x,random_pos.y);
    game->start();
    delete game;
    CloseWindow();
    return 0;
}