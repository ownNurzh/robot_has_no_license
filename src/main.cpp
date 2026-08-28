
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <array>

#include "raylib.h"


struct Collision {
    float angle;//deg
    float hit_distance;
};

struct Border {
    Vector2 start;
    Vector2 end;
};

constexpr int COLLISION_COUNT {8};

constexpr float COLLISIONS_ANGLE[COLLISION_COUNT] {0.0f,90.0f,180.0f,270.0f,45.0f,135.0f,225.0f,315.0f};

constexpr int COLLISION_DISTANCE {50};

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 900;

constexpr int ROAD_WIDTH {150};

constexpr int SPAWN_SIZE = ROAD_WIDTH * 0.6f;

constexpr int SPAWN_SIZE_DIFF = ROAD_WIDTH * 0.1f;

constexpr int ROADS_GAP {ROAD_WIDTH};

constexpr float CAR_WIDTH {50.0f};

constexpr float CAR_HEIGHT {20.0f};


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

float cross(Vector2 a,Vector2 b) {
    return a.x * b.y - a.y * b.x;
}

constexpr Rectangle city_roads[] {
    {ROADS_GAP,0,ROAD_WIDTH,SCREEN_HEIGHT},

    {SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,0,ROAD_WIDTH,SCREEN_HEIGHT},


    {0,ROADS_GAP,SCREEN_WIDTH,ROAD_WIDTH},

    {0,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH,SCREEN_WIDTH,ROAD_WIDTH},
};

constexpr Border road_borders[] {
    //vertical borders
    {{ROADS_GAP,0},{ROADS_GAP,ROADS_GAP}},
    {{ROADS_GAP + ROAD_WIDTH,0},{ROADS_GAP + ROAD_WIDTH,ROADS_GAP}},

    {{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,0},{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,ROADS_GAP}},
    {{SCREEN_WIDTH - ROADS_GAP,0},{SCREEN_WIDTH - ROADS_GAP,ROADS_GAP}},

    {{ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP},{ROADS_GAP,SCREEN_HEIGHT}},
    {{ROADS_GAP + ROAD_WIDTH,SCREEN_HEIGHT - ROADS_GAP},{ROADS_GAP + ROAD_WIDTH,SCREEN_HEIGHT}},

    {{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,SCREEN_HEIGHT - ROADS_GAP},{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,SCREEN_HEIGHT}},
    {{SCREEN_WIDTH - ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP},{SCREEN_WIDTH - ROADS_GAP,SCREEN_HEIGHT}},

    {{ROADS_GAP,ROADS_GAP + ROAD_WIDTH},{ROADS_GAP,ROADS_GAP + ROAD_WIDTH + SCREEN_HEIGHT - ROADS_GAP * 2 - ROAD_WIDTH * 2}},
    {{ROADS_GAP + ROAD_WIDTH ,ROADS_GAP + ROAD_WIDTH},{ROADS_GAP + ROAD_WIDTH,ROADS_GAP + ROAD_WIDTH + SCREEN_HEIGHT - ROADS_GAP * 2 - ROAD_WIDTH * 2}},

    {{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,ROADS_GAP + ROAD_WIDTH},{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,ROADS_GAP + ROAD_WIDTH + SCREEN_HEIGHT - ROADS_GAP * 2 - ROAD_WIDTH * 2}},
    {{SCREEN_WIDTH - ROADS_GAP ,ROADS_GAP + ROAD_WIDTH},{SCREEN_WIDTH - ROADS_GAP,ROADS_GAP + ROAD_WIDTH + SCREEN_HEIGHT - ROADS_GAP * 2 - ROAD_WIDTH * 2}},

    {{0,ROADS_GAP},{0,ROADS_GAP + ROAD_WIDTH}},
    {{0,SCREEN_HEIGHT - ROADS_GAP},{0,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH}},

    {{SCREEN_WIDTH,ROADS_GAP},{SCREEN_WIDTH,ROADS_GAP + ROAD_WIDTH}},
    {{SCREEN_WIDTH,SCREEN_HEIGHT - ROADS_GAP},{SCREEN_WIDTH,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH}},

    //horizontal borders
    {{0,ROADS_GAP},{ROADS_GAP,ROADS_GAP}},
    {{0,ROADS_GAP + ROAD_WIDTH},{ROADS_GAP,ROADS_GAP + ROAD_WIDTH}},

    {{SCREEN_WIDTH - ROADS_GAP,ROADS_GAP},{SCREEN_WIDTH,ROADS_GAP}},
    {{SCREEN_WIDTH - ROADS_GAP,ROADS_GAP + ROAD_WIDTH},{SCREEN_WIDTH,ROADS_GAP + ROAD_WIDTH}},

    {{SCREEN_WIDTH - ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP },{SCREEN_WIDTH,SCREEN_HEIGHT - ROADS_GAP}},

    {{SCREEN_WIDTH - ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH},{SCREEN_WIDTH,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH }},

    {{0,SCREEN_HEIGHT - ROADS_GAP},{ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP}},
    {{0,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH },{ROADS_GAP,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH}},

    {{ROADS_GAP + ROAD_WIDTH,ROADS_GAP},{ROADS_GAP + ROAD_WIDTH + SCREEN_WIDTH - ROADS_GAP * 2 - ROAD_WIDTH * 2,ROADS_GAP}},
    {{ROADS_GAP + ROAD_WIDTH,ROADS_GAP + ROAD_WIDTH},{ROADS_GAP + ROAD_WIDTH + SCREEN_WIDTH - ROADS_GAP * 2 - ROAD_WIDTH * 2,ROADS_GAP + ROAD_WIDTH}},

    {{ROADS_GAP + ROAD_WIDTH,SCREEN_HEIGHT - ROADS_GAP},{ROADS_GAP + ROAD_WIDTH + SCREEN_WIDTH - ROADS_GAP * 2 - ROAD_WIDTH * 2,SCREEN_HEIGHT - ROADS_GAP}},
    {{ROADS_GAP + ROAD_WIDTH,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH},{ROADS_GAP + ROAD_WIDTH + SCREEN_WIDTH - ROADS_GAP * 2 - ROAD_WIDTH * 2,SCREEN_HEIGHT - ROADS_GAP - ROAD_WIDTH}},

    {{ROADS_GAP,0 },{ROADS_GAP + ROAD_WIDTH,0}},
    {{SCREEN_WIDTH - ROADS_GAP,0 },{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,0}},

    {{ROADS_GAP,SCREEN_HEIGHT},{ROADS_GAP + ROAD_WIDTH,SCREEN_HEIGHT}},
    {{SCREEN_WIDTH - ROADS_GAP,SCREEN_HEIGHT },{SCREEN_WIDTH - ROADS_GAP - ROAD_WIDTH,SCREEN_HEIGHT}},
};

constexpr Rectangle spawn_points[] {
    {city_roads[0].x + (ROAD_WIDTH - SPAWN_SIZE) / 2,city_roads[0].y + SPAWN_SIZE_DIFF,SPAWN_SIZE,SPAWN_SIZE},
    {city_roads[0].x + (ROAD_WIDTH - SPAWN_SIZE) / 2,city_roads[0].y + city_roads[0].height - SPAWN_SIZE - SPAWN_SIZE_DIFF,SPAWN_SIZE,SPAWN_SIZE},

    {city_roads[1].x + (ROAD_WIDTH - SPAWN_SIZE) / 2,city_roads[1].y + SPAWN_SIZE_DIFF,SPAWN_SIZE,SPAWN_SIZE},
    {city_roads[1].x + (ROAD_WIDTH - SPAWN_SIZE) / 2,city_roads[1].y + city_roads[1].height - SPAWN_SIZE - SPAWN_SIZE_DIFF,SPAWN_SIZE,SPAWN_SIZE},

    {city_roads[2].x + SPAWN_SIZE_DIFF,city_roads[2].y + (ROAD_WIDTH - SPAWN_SIZE) / 2,SPAWN_SIZE,SPAWN_SIZE},
    {city_roads[2].x + city_roads[2].width - SPAWN_SIZE - SPAWN_SIZE_DIFF,city_roads[2].y + (ROAD_WIDTH - SPAWN_SIZE) / 2,SPAWN_SIZE,SPAWN_SIZE},

    {city_roads[3].x + SPAWN_SIZE_DIFF,city_roads[3].y + (ROAD_WIDTH - SPAWN_SIZE) / 2,SPAWN_SIZE,SPAWN_SIZE},
    {city_roads[3].x + city_roads[3].width - SPAWN_SIZE - SPAWN_SIZE_DIFF,city_roads[3].y + (ROAD_WIDTH - SPAWN_SIZE) / 2,SPAWN_SIZE,SPAWN_SIZE},
};

Vector2 get_random_spawn() {
    int random_index = GetRandomValue(0,4 * 2 - 1);
    return {spawn_points[random_index].x + SPAWN_SIZE / 2,spawn_points[random_index].y + SPAWN_SIZE / 2};
}

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

    Car(float x,float y,float w,float h):position{x,y},size{w,h} {

    }
    std::array<Collision,COLLISION_COUNT> get_collisions() {
        std::array<Collision,COLLISION_COUNT> result {};
        for (int i {0};i < COLLISION_COUNT;i++) {

            float angle = COLLISIONS_ANGLE[i];

            Vector2 direction {get_direction_from_angle(this->angle + angle)};
            result[i] = {angle,COLLISION_DISTANCE};

            for (const Border &border : road_borders) {  
                Vector2 line_dir {border.end.x - border.start.x,border.end.y - border.start.y};
                Vector2 to_line {border.start.x - this->position.x,border.start.y - this->position.y};
                float denominator {cross(direction,line_dir)};

                if (denominator == 0.0f)
                    continue;

                float t {cross(to_line,line_dir) / denominator};
                float u {cross(to_line,direction) / denominator};
                if (t > 0.0f && u > 0.0f && u < 1.0f) {
                    result[i].hit_distance = std::min(t,result[i].hit_distance);
                }
                
            }
        }
        return result;
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
        this->speed = std::clamp(this->speed + acceleration * input * delta_time,-this->max_speed * 0.5f,this->max_speed);

    }
    void turn(float hw,float delta_time) {
        float input = std::clamp(hw,-1.0f,1.0f);
        float r = input * turn_speed * delta_time;
        this->steering_angle = std::clamp(this->steering_angle + r,-30.0f,30.0f);
    }
    static void draw_car(Car car,bool dev = false) {
        Rectangle temp_car {car.position.x,car.position.y,car.size.x,car.size.y};
        Vector2 car_direction {get_direction_from_angle(car.angle)};
        float d_x = car.size.x / 2;

        Rectangle temp_up_car {car.position.x,car.position.y,car.size.x * 0.5f,car.size.y * 0.8f};

        Vector2 wheels_size {car.size.x * 0.1f,car.size.y * 1.3f};

        float wheels_kf = 0.75f;

        Rectangle front_wheels {car.position.x + car_direction.x * d_x * wheels_kf,car.position.y + car_direction.y * d_x * wheels_kf,wheels_size.x,wheels_size.y};

        Rectangle back_wheels {car.position.x - car_direction.x * d_x * wheels_kf,car.position.y - car_direction.y * d_x * wheels_kf,wheels_size.x,wheels_size.y};

        DrawRectanglePro(front_wheels, {front_wheels.width / 2 ,front_wheels.height / 2},-(car.angle + car.steering_angle), BLACK);

        DrawRectanglePro(back_wheels, {back_wheels.width / 2 ,back_wheels.height / 2},-car.angle, BLACK);

        DrawRectanglePro(temp_car, {car.size.x / 2 ,car.size.y / 2},-car.angle, YELLOW);

        DrawRectanglePro(temp_up_car, {temp_up_car.width / 2 ,temp_up_car.height / 2},-car.angle, GOLD);

        if (dev) {
            //car angle
            
            Vector2 end_pos {car.position.x + car_direction.x * d_x,car.position.y + car_direction.y * d_x};
            DrawLine(car.position.x,car.position.y,end_pos.x,end_pos.y,BLACK);

            //rudder
            Vector2 rudder {get_direction_from_angle(car.angle + car.steering_angle)};
            
            DrawLine(end_pos.x,end_pos.y ,end_pos.x + rudder.x * 30,end_pos.y + rudder.y * 30,PURPLE);


            float steering_radius {car.size.x / std::tanf(to_radians(car.steering_angle))};
            DrawCircleLines(car.position.x + car_direction.y * steering_radius,car.position.y + -(car_direction.x) * steering_radius,steering_radius,BLACK);


            // collision rays
            auto collisions = car.get_collisions();
            for (const Collision &collision : collisions) {
                Vector2 direction {get_direction_from_angle(car.angle + collision.angle)};
                DrawLine(car.position.x,car.position.y ,car.position.x + direction.x * COLLISION_DISTANCE,car.position.y + direction.y * COLLISION_DISTANCE,RED);
                DrawLine(car.position.x,car.position.y ,car.position.x + direction.x * collision.hit_distance,car.position.y + direction.y * collision.hit_distance,GREEN);
            }
        }
    }
};


void draw_city() {
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

int main()
{
    SetRandomSeed(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Robot has no license");

    SetTargetFPS(60);

    Vector2 random_pos = get_random_spawn();
    Car main_car {random_pos.x,random_pos.y,50.0f,20.0f};

    while (!WindowShouldClose())
    {   
        float delta_time = GetFrameTime();
        if (IsKeyDown(KEY_S)) main_car.gas(-0.5f,delta_time);
        else if (IsKeyDown(KEY_W)) main_car.gas(0.5f,delta_time);
        if (IsKeyDown(KEY_D)) main_car.turn(-0.9f,delta_time);
        else if (IsKeyDown(KEY_A)) main_car.turn(0.9f,delta_time);

        main_car.update(delta_time);
        BeginDrawing();
        draw_city();
       

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