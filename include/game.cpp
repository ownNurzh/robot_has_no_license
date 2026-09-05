#include <ctime>

#include "game.h"
#include "game_datas.h"
#include "car.h"

void Game::draw_city() {
    ClearBackground(DARKGREEN);

    for (const Rectangle &road : city_roads) {
        DrawRectangleRec(road,DARKGRAY);
        if (road.width == ROAD_WIDTH) {
            DrawLineDashed({road.x + ROAD_WIDTH / 2,road.y},{road.x + ROAD_WIDTH / 2,road.y + road.height},10,6,WHITE);
        } else {
            DrawLineDashed({road.x ,road.y + ROAD_WIDTH / 2},{road.x + road.width,road.y + ROAD_WIDTH / 2},10,6,WHITE);
        }
    }

    for (const Rectangle &spawn : spawn_points) {
        DrawRectangleRec(spawn,DARKPURPLE);
    }

    for (const Border &border : road_borders) {
        DrawLineV(border.start,border.end,MAGENTA);
    }
}

void Game::start() {
    SetRandomSeed(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Robot has no license");

    SetTargetFPS(60);

    Vector2 random_pos = get_random_spawn();
    Vector2 *goal = new Vector2{get_random_spawn()};
    Car main_car {random_pos.x,random_pos.y,50.0f,20.0f};
    while (!WindowShouldClose())
    {   
        float delta_time = GetFrameTime();
        if (IsKeyDown(KEY_S)) main_car.gas(-0.5f,delta_time);
        else if (IsKeyDown(KEY_W)) main_car.gas(0.5f,delta_time);
        if (IsKeyDown(KEY_D)) main_car.turn(-0.9f,delta_time);
        else if (IsKeyDown(KEY_A)) main_car.turn(0.9f,delta_time);
        Vector2 diff_with_car_goal_pos = Vector2{goal->x - main_car.position.x,goal->y - main_car.position.y};
        float d = SPAWN_SIZE / 2;
        if (abs(diff_with_car_goal_pos.x) < d && abs(diff_with_car_goal_pos.y) < d) {
            delete goal;
            goal = new Vector2{get_random_spawn()};
        }

        main_car.update(delta_time);
        BeginDrawing();
        draw_city();
        DrawRectanglePro({goal->x,goal->y,SPAWN_SIZE,SPAWN_SIZE},{SPAWN_SIZE / 2 , SPAWN_SIZE / 2},0,ORANGE);

        Car::draw_car(main_car,true);
        DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 10, 20, WHITE);
        DrawText(TextFormat("Fps: %i", GetFPS()), 10, 30, 20, WHITE);
        DrawText(TextFormat("ANGLE: %f", main_car.angle), 10, 50, 20, WHITE);
        DrawText(TextFormat("SPEED: %f", main_car.speed), 10, 70, 20, WHITE);
        EndDrawing();


    }
    delete goal;
    CloseWindow();
}

Vector2 Game::get_random_spawn() {
    //todo
    int random_index = GetRandomValue(0,4 * 2 - 1);
    float halp_spawn_size = SPAWN_SIZE / 2;
    return {spawn_points[random_index].x + halp_spawn_size,spawn_points[random_index].y + halp_spawn_size};
}