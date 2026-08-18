
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "raylib.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;

float to_radians(float deg) {
    return deg * (3.1415926f / 180.0f);
}

class Car {
public:
    Vector2 velocity {0.0f,0.0f};
    Vector2 position;
    Vector2 size;

    float acceleration {0.0f};
    float speed {0.0f};
    float rudder_angle {0.0f};
    float angle {0.0f};

    Car(float x,float y,float w,float h):position{x,y},size{w,h} {

    }
    void update(float delta_time) {
        this->position.x += this->velocity.x * delta_time;
        this->position.y += this->velocity.y * delta_time;
    }
    void gas() {

    }
    void turn(float hw) {
        float input = std::max(-1.0f,std::min(hw,1.0f));
        this->rudder_angle = std::max(-60.0f,std::min(this->rudder_angle + (input * 30.0f),60.0f));
    }
    static void draw_car(Car car,bool dev = false) {
        Rectangle temp_rect = {car.position.x,car.position.y,car.size.x,car.size.y};
        DrawRectanglePro(temp_rect, {0.0f,0.0f},car.angle, MAROON);
        if (dev) {
            Vector2 direction {std::cos(to_radians(car.rudder_angle + 90)),-std::sin(to_radians(car.rudder_angle + 90))};
            std::cout << direction.x << "," << direction.y << "\n";
            float start_x = car.position.x + car.size.x / 2;
            DrawLine(start_x,car.position.y,start_x + (direction.x * 30),car.position.y + (direction.y * 30),BLACK);
        }
    }
};

int main()
{
    SetRandomSeed(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Diagram Voronoi");

    SetTargetFPS(60);


    Car main_car {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,40.0f,100.0f};


    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_S)) ;
        else if (IsKeyDown(KEY_W)) ;
        if (IsKeyDown(KEY_D)) main_car.turn(-0.2);
        else if (IsKeyDown(KEY_A)) main_car.turn(0.2);




        BeginDrawing();

        ClearBackground(DARKGREEN);

        Car::draw_car(main_car,true);
        DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 10, 20, WHITE);
        DrawText(TextFormat("Fps: %i", GetFPS()), 10, 30, 20, WHITE);
        DrawText("Car:", 10, 65, 20, MAROON);
        DrawText(TextFormat("angle: %f",main_car.angle), 10, 90, 20, MAROON);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}