
#include <iostream>
#include <ctime>

#include "raylib.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;

constexpr double G = 9.80665;

class Car {
public:
    float velocity {10.0f};
    float m {50.0f};
    float a {5.0f};
    float f {250.0f};
    float angle {0.0f};
    float rudder {0.0f};
    float w {5.0f};
    float u {1.0f};


    Vector2 position;
    Vector2 size;
    Car(float x,float y,float w,float h):position{x,y},size{w,h} {

    }
    void update(float delta_time) {

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
        //if (IsKeyDown(KEY_S)) std::cout << "TEST" << "\n";
        // else if (IsKeyDown(KEY_W)) car_y -= 3.0f;
        // if (IsKeyDown(KEY_D)) car_x += 3.0f;
        // else if (IsKeyDown(KEY_A)) car_x-= 3.0f;




        BeginDrawing();

        ClearBackground(DARKGREEN);

        DrawRectangleV(main_car.position, main_car.size, MAROON);
        DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 10, 20, WHITE);
        DrawText(TextFormat("Fps: %i", GetFPS()), 10, 30, 20, WHITE);
        DrawText("Car:", 10, 65, 20, MAROON);
        DrawText(TextFormat("rudder: %f",main_car.rudder), 10, 90, 20, MAROON);
        DrawText(TextFormat("angle: %f",main_car.angle), 10, 110, 20, MAROON);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}