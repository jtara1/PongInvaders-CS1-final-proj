#ifndef PONGINVADERS_H
#define PONGINVADERS_H

#define ALLEGRO_STATICLINK
#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_image.h"

#include <vector>
#include <time.h>
#include <cmath>
#include <iostream>
#include <string>

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

class Egg
{
public:
    /* Generates an egg at random position on the screen */
    Egg();

    int get_x() const {return cx;}
    int get_y() const {return cy;}

    /* Draws the egg with the Allegro library */
    void draw();

    /* Returns coordinates at the center of the egg */
    std::vector<int> get_coord();

protected:
    // radii
    int rx, ry;

    // center
    int cx, cy;

    std::vector<int> egg_info;

    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
};


class Ball: public Egg
{
public:
    /* Does the same thing the Egg() constructor does, and defines more variables */
    Ball();

    int get_radius() const {return rx;}

    /* Increases the values of cx and cy by spd_x and spd_y */
    void update();

    /* Changes velocity based on parameter given */
    void hit_wall(int wall);

    /* Increases the speed */
    void incr_speed();

    /* Draws the ball */
    void draw();

protected:
    float spd_x, spd_y;
};


class Enemy: public Ball
{
public:
    Enemy() {Ball(); rx = 30; enemy_bmp = al_load_bitmap("enemy.gif");}

    void draw();

private:
    ALLEGRO_BITMAP* enemy_bmp;

};


class Laser
{
public:
    Laser(int paddle_x, int paddle_y);

    int get_y1() const {return y1;}
    int get_x2() const {return x2;}
    int get_y2() const {return y2;}

    /* Adds the value of each speed to each coordinate */
    void update();

    void draw();

private:
    int x1, y1, x2, y2;
    float spd_x, spd_y;
    int len;
    int line_thickness;
    ALLEGRO_COLOR red = al_map_rgb(255,50,50);
};

#endif // PONGINVADERS_H
