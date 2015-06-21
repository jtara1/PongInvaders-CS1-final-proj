#include "PongInvaders.h"

using namespace std;

Egg::Egg() {
    srand(time(NULL));

    // radii of the ellipse/egg
    rx = 15;
    ry = 20;

    // center of ellipse/egg
    cx = rand() % (DISPLAY_WIDTH - rx) + rx; // gives an int between 20 and 1260
    cy = rand() % (DISPLAY_HEIGHT/3) + ry; // gives an int between 30 and 640
}

void Egg::draw() {
    al_draw_filled_ellipse(cx, cy, rx - 1, ry - 1, white);
    al_draw_ellipse(cx, cy, rx, ry, black, 1);
}

vector<int> Egg::get_coord() {
    egg_info.push_back(cx);
    egg_info.push_back(cy);
    return egg_info;
}


Ball::Ball() {
    Egg();

    srand(time(NULL));
    int rand_dir_x = rand() % 2;
    int rand_dir_y = rand() % 2;
    //cout << rand_dir_x << " " << rand_dir_y << " "; // debug
    if (rand_dir_x == 0) rand_dir_x = -1;
    if (rand_dir_y == 0) rand_dir_y = -1;

    spd_x = 2 * rand_dir_x; spd_y = 2 * rand_dir_y;
}

void Ball::update() {
    cx += spd_x; cy += spd_y;
}

void Ball::hit_wall(int wall) {
/*
    Key:
    0: vertical wall
    1: horizontal wall
*/
    switch(wall) {
    case(0):
        spd_x = spd_x * -1;
        break;
    case(1):
        spd_y = spd_y * -1;
        break;
    }
}

void Ball::incr_speed() {
    if (spd_x < 6 and spd_y < 6) {
        if (spd_x == abs(spd_x)) spd_x += 1;
        else spd_x -= 1;

        if (spd_y == abs(spd_y)) spd_y += 1;
        else spd_y -= 1;
    }
}

void Ball::draw() {
    al_draw_filled_circle(cx, cy, rx - 1, white);
    al_draw_circle(cx, cy, rx, black, 2);
}


Laser::Laser(int paddle_x, int paddle_y) {
    len = 15; // length of line
    x1 = paddle_x;
    y1 = paddle_y;
    x2 = paddle_x;
    y2 = paddle_y + len;

    spd_x = 0;
    spd_y = -5;
    line_thickness = 7;
}


void Laser::update() {
    x1 += spd_x;
    y1 += spd_y;
    x2 += spd_x;
    y2 += spd_y;
}

void Laser::draw() {
    al_draw_line(x1, y1, x2, y2, red, line_thickness);
}


void Enemy::draw() {
    al_draw_bitmap(enemy_bmp, cx, cy, 0);
}
