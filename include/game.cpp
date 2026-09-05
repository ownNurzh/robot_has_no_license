#include "game.h"
#include "game_datas.h"
#include "car.h"

// Game::~Game() {
//     delete goal_position;
// }

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

void Game::draw_cars() {
    for (const Car &car : this->cars) {
        Car::draw_car(car,true);
    }
}



void Game::add_car(float x,float y) {
    cars.push_back({x,y,50.0f,20.0f});
}

void Game::loop() {
    float delta_time = GetFrameTime();

    BeginDrawing();
    this->draw_city();

    float half_spawn_size = SPAWN_SIZE / 2;
    DrawRectanglePro({goal_position->x,goal_position->y,SPAWN_SIZE,SPAWN_SIZE},{half_spawn_size, half_spawn_size},0,ORANGE);

    this->draw_cars();

    DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 10, 20, WHITE);
    DrawText(TextFormat("Fps: %i", GetFPS()), 10, 30, 20, WHITE);

    EndDrawing();
    for (Car &car : this->cars) {
        car.update(delta_time);
    }
}

void Game::start() {
    
    goal_position = new Vector2{get_random_spawn()};
    while (!WindowShouldClose())
    {   
        float delta_time = GetFrameTime();
        if (IsKeyDown(KEY_S)) this->cars[0].gas(-0.5f,delta_time);
        else if (IsKeyDown(KEY_W)) this->cars[0].gas(0.5f,delta_time);
        if (IsKeyDown(KEY_D)) this->cars[0].turn(-0.9f,delta_time);
        else if (IsKeyDown(KEY_A)) this->cars[0].turn(0.9f,delta_time);
        Vector2 diff_with_car_goal_pos = Vector2{goal_position->x - this->cars[0].position.x,goal_position->y - this->cars[0].position.y};
        float d = SPAWN_SIZE / 2;
        if (abs(diff_with_car_goal_pos.x) < d && abs(diff_with_car_goal_pos.y) < d) {
            delete goal_position;
            goal_position = new Vector2{get_random_spawn()};
        }
        this->loop();
    }
}

Vector2 Game::get_random_spawn() {
    //todo
    int random_index = GetRandomValue(0,4 * 2 - 1);
    float halp_spawn_size = SPAWN_SIZE / 2;
    return {spawn_points[random_index].x + halp_spawn_size,spawn_points[random_index].y + halp_spawn_size};
}