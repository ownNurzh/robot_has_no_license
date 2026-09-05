#include <algorithm>

#include <car.h>

Car::Car(float x,float y,float w,float h):position{x,y},size{w,h} {}

std::array<Collision,COLLISION_COUNT> Car::get_collisions() const {
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

void Car::update(float delta_time) {
    if (!alive) return;
    if (angle >= 360.0f)
        angle -= 360.0f;
    if (angle < 0.0f)
        angle += 360.0f;
    auto collisions = this->get_collisions();
    for (const Collision &collision : collisions) {
        Vector2 local_direction {get_direction_from_angle(collision.angle)};
        float tx = this->size.x / 2 / fabsf(local_direction.x);
        float ty = this->size.y / 2 / fabsf(local_direction.y);
        float t = fminf(tx, ty);
        if (collision.hit_distance <= t) {
            this->alive = false;
        }
    }
    if (this->speed != 0.0f) {
        float steering_radius {this->size.x / std::tanf(to_radians(this->steering_angle))};
        float angular_velocity {this->speed / steering_radius };
        Vector2 direction = get_direction_from_angle(this->angle);
        this->position.x += this->speed * direction.x * delta_time;
        this->position.y += this->speed * direction.y * delta_time;
        this->angle += to_deg(angular_velocity) * delta_time;
    }
}

void Car::gas(float hw,float delta_time) {
    float input = std::clamp(hw,-1.0f,1.0f);
    this->speed = std::clamp(this->speed + acceleration * input * delta_time,-this->max_speed * 0.2f,this->max_speed);

}
void Car::turn(float hw,float delta_time) {
    float input = std::clamp(hw,-1.0f,1.0f);
    float r = input * turn_speed * delta_time;
    this->steering_angle = std::clamp(this->steering_angle + r,-30.0f,30.0f);
}

void Car::draw_car(const Car &car,bool dev) {
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
            Vector2 local_direction {get_direction_from_angle(collision.angle)};
            float tx = car.size.x / 2 / fabsf(local_direction.x);
            float ty = car.size.y / 2 / fabsf(local_direction.y);
            float t = fminf(tx, ty);
            DrawLine(car.position.x,car.position.y ,car.position.x + direction.x * t,car.position.y + direction.y * t,PINK);
        }
    }
}