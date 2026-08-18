
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "raylib.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;

float to_radians(float deg) {
    constexpr double df = PI / 180.0f;
    return deg * df;
}

float to_deg(float rad) {
    constexpr double df = (180.0f / PI);
    return rad * df;
}

Vector2 get_direction_from_angle(float angle) {
    float radians = to_radians(angle);
    return {cos(radians) ,-sin(radians)};
}

class Car {
public:
    Vector2 position;
    Vector2 size;

    float acceleration {100.0f};
    float speed {0.0f};
    float turn_speed {90.0f};
    float steering_angle {0.0f};
    float angle {0.0f};

    Car(float x,float y,float w,float h):position{x,y},size{w,h} {

    }
    void update(float delta_time) {
        if (angle >= 360.0f)
            angle -= 360.0f;
        if (angle < 0.0f)
            angle += 360.0f;

        if (this->speed != 0.0f) {
            float steering_radius {this->size.x / std::tanf(to_radians(this->steering_angle))};
            float angular_velocity {this->speed / steering_radius };
            Vector2 direction = get_direction_from_angle(this->angle);
            this->position.x += this->speed * direction.x * delta_time;
            this->position.y += this->speed * direction.y * delta_time;
            this->angle += to_deg(angular_velocity) * delta_time;
        }
    }
    void gas(float hw,float delta_time) {
        float input = std::clamp(hw,-1.0f,1.0f);
        this->speed = std::min(100.0f,this->speed + acceleration * input * delta_time);

    }
    void turn(float hw,float delta_time) {
        float input = std::clamp(hw,-1.0f,1.0f);
        float r = input * turn_speed * delta_time;
        this->steering_angle = std::clamp(this->steering_angle + r,-45.0f,45.0f);
    }
    static void draw_car(Car car,bool dev = false) {
        Rectangle temp_rect = {car.position.x,car.position.y,car.size.x,car.size.y};
        DrawRectanglePro(temp_rect, {car.size.x / 2 ,car.size.y / 2},-car.angle, MAROON);
        if (dev) {
            //car angle
            Vector2 car_direction {get_direction_from_angle(car.angle)};
            float d_x = car.size.x / 2;
            Vector2 end_pos {car.position.x + car_direction.x * d_x,car.position.y + car_direction.y * d_x};
            DrawLine(car.position.x,car.position.y,end_pos.x,end_pos.y,BLACK);

            //rudder
            Vector2 rudder {get_direction_from_angle(car.angle + car.steering_angle)};
            DrawLine(end_pos.x,end_pos.y ,end_pos.x + rudder.x * 30,end_pos.y + rudder.y * 30,PURPLE);


            float steering_radius {car.size.x / std::tanf(to_radians(car.steering_angle))};
            DrawCircleLines(car.position.x + car_direction.y * steering_radius,car.position.y + -(car_direction.x) * steering_radius,steering_radius,BLACK);
        }
    }
};

int main()
{
    SetRandomSeed(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Robot has no license");

    SetTargetFPS(60);


    Car main_car {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,100.0f,40.0f};

    while (!WindowShouldClose())
    {   
        float delta_time = GetFrameTime();
        if (IsKeyDown(KEY_S)) main_car.gas(-0.5f,delta_time);
        else if (IsKeyDown(KEY_W)) main_car.gas(0.5f,delta_time);
        if (IsKeyDown(KEY_D)) main_car.turn(-0.9f,delta_time);
        else if (IsKeyDown(KEY_A)) main_car.turn(0.9f,delta_time);

        main_car.update(delta_time);
        BeginDrawing();

        ClearBackground(DARKGREEN);

        Car::draw_car(main_car,true);
        DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 10, 20, WHITE);
        DrawText(TextFormat("Fps: %i", GetFPS()), 10, 30, 20, WHITE);
        DrawText(TextFormat("ANGLE: %f", main_car.angle), 10, 50, 20, WHITE);
        DrawText(TextFormat("SPEED: %f", main_car.speed), 10, 70, 20, WHITE);
        EndDrawing();


    }
    CloseWindow();
    return 0;
}