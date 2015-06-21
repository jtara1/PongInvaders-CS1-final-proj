#include "PongInvaders.h"

using namespace std;

int main()
{
    const int width50p = DISPLAY_WIDTH / 2;
    const int height50p = DISPLAY_HEIGHT / 2;
    const float FPS = 100.0;
    int enemy_spawn_rate = 200;
    int ball_spawn_rate = 1200;
    int score = 0;
    int lives = 5;
    string score_str;
    string lives_str;

    al_init();

    ALLEGRO_COLOR green = al_map_rgb(65, 191, 70);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);

    ALLEGRO_DISPLAY *display = NULL;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    //ALLEGRO_FONT *font = al_load_font("AGENCYB.ttf", 36, 4);

    al_set_new_display_flags(ALLEGRO_NOFRAME);
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_set_window_title(display, "SomeGame by James Taracevicz");

    al_init_image_addon();
    //al_init_font_addon();
    //al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    bool done = false, draw = true;
    int x = width50p, y = DISPLAY_HEIGHT - 21; // x, y will be paddle coordinates
    int paddle_offset_x = 150, paddle_offset_y = 20; // these help in drawing paddle and checking hit box
    int movespeed = 10;
    //int cursor_x = width50p, cursor_y = height50p; // cursor_x, cursor_y will be mouse coordinates
    //double cursor_radius = 10.0;

    al_hide_mouse_cursor(display);
    ALLEGRO_KEYBOARD_STATE keystate;

    vector<Ball*> ball_list;
    Ball* a_ball = new Ball();
    ball_list.push_back(a_ball);
    int balls_lost = 0;
    bool delete_this;
    int ball_x, ball_y;
    int ball_r;

    vector<Laser*> laser_list;
    int laser_x, laser_y;

    vector<Enemy*> enemy_list;
    Enemy* enemy = new Enemy();
    enemy_list.push_back(enemy);
    int enemy_x, enemy_y;
    int enemy_r;

    int loop_counter = 0;
    double cur_time;
    double prev_time = al_get_time();
    double time_gap;

    al_start_timer(timer);
    while(!done)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if (events.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            /* Asks to exit the game if escape key is pressed */
            {
                //int i = al_show_native_message_box(display, "Confirm Exit", "Game Paused,", "Exit the game?", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
                //al_show_mouse_cursor(display);
                //if (i == 1) {done = true;}
                done = true;
            }
        }

        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        /* Exits the game if the x button is pressed */
        {
            done = true;
        }

        else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (events.mouse.button == 1) {
                cur_time = al_get_time();
                time_gap = cur_time - prev_time;
                if (time_gap >= 0.15) {
                    Laser* laser = new Laser(x + (paddle_offset_x/2), y + (paddle_offset_y/2));
                    laser_list.push_back(laser);
                    laser->draw();
                    prev_time = al_get_time();
                }
            }
        }

        else if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //cursor_x = events.mouse.x;
            //cursor_y = events.mouse.y;
        }

        else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            al_get_keyboard_state(&keystate);
            if (al_key_down(&keystate, ALLEGRO_KEY_SPACE)) {

            }
            // Adds a new ball on the screen when e is pressed
            if (al_key_down(&keystate, ALLEGRO_KEY_E)) {
                if (ball_list.size() < 4) {
                    Ball* ball = new Ball();
                    ball_list.push_back(ball);
                }
            }
        }

        else if (events.type == ALLEGRO_EVENT_TIMER)
        {
            loop_counter++;
            if (loop_counter % ball_spawn_rate  == 0) {
                Ball* ball = new Ball();
                ball_list.push_back(ball);
            }
            /* This loop handles ball physics */
            for (unsigned int i = 0; i < ball_list.size(); i++) {
                delete_this = false;
                Ball* ball = ball_list[i];
                ball_x = ball->get_x();
                ball_y = ball->get_y();
                ball_r = ball->get_radius();
//                ball_x += ball_speed * dir_modifier_x;
//                ball_y += ball_speed * dir_modifier_y;
                if (ball_y - ball_r <= 0) {
                    ball->hit_wall(1);
                }
                if ((ball_x - ball_r <= 0) or (ball_x + ball_r >= DISPLAY_WIDTH)) {
                    ball->hit_wall(0);
                }
                if (ball_y + ball_r >= y) {
                    if (ball_x >= x and ball_x <= x + paddle_offset_x) {
                        ball->hit_wall(1);
                        ball->incr_speed();
                        score += 50;
                    }
                    else {delete_this = true; score -= 100; }
                }
                if (delete_this) {
                    ball_list.erase(ball_list.begin() + i);
                    delete ball;
                    balls_lost++;
                }
            }

            /* This loop handles enemy physics */
            if (loop_counter % enemy_spawn_rate  == 0) {
                Enemy* enemy = new Enemy();
                enemy_list.push_back(enemy);
            }
            for (unsigned int i = 0; i < enemy_list.size(); i++) {
                delete_this = false;
                Enemy* enemy = enemy_list[i];
                enemy_x = enemy->get_x();
                enemy_y = enemy->get_y();
                enemy_r = enemy->get_radius();
                for (unsigned int k = 0; k < laser_list.size(); k++) {
                    Laser* laser = laser_list[k];
                    laser_x = laser->get_x2();
                    laser_y = laser->get_y2();

                    /* Checks for laser + enemy collision */
                    if ((laser_x <= enemy_x + enemy_r and laser_x >= enemy_x - enemy_r) and
                        (laser_y <= enemy_y + enemy_r and laser_y >= enemy_y - enemy_r)) {
                            laser_list.erase(laser_list.begin() + k);
                            delete laser;
                            enemy_list.erase(enemy_list.begin() + i);
                            delete enemy;
                            score += 5;
                        }
                }
                if (enemy_y - enemy_r <= 0) {
                    enemy->hit_wall(1);
                }
                if ((enemy_x - enemy_r <= 0) or (enemy_x + enemy_r >= DISPLAY_WIDTH)) {
                    enemy->hit_wall(0);
                }
                if (enemy_y + enemy_r >= y) {
                    if (enemy_x >= x and enemy_x <= x + paddle_offset_x) {
                        enemy->hit_wall(1);
                        score += 50;
                    }
                    else {delete_this = true; score -= 100; }
                }
                if (delete_this) {
                    enemy_list.erase(enemy_list.begin() + i);
                    delete enemy;
                    lives--;
                }
            }

            /* This block handles movement for the paddle */
            al_get_keyboard_state(&keystate);
            if (al_key_down(&keystate, ALLEGRO_KEY_LSHIFT)) {
                movespeed = 20;
            }
            else movespeed = 10;
            if (al_key_down(&keystate, ALLEGRO_KEY_D)) {
                if (x + paddle_offset_x < DISPLAY_WIDTH)
                    x += movespeed;
            }
            else if (al_key_down(&keystate, ALLEGRO_KEY_A)) {
                if (x > 0 )
                    x -= movespeed;
            }

            /* If player reaches edge of screen, game over */
            draw = true;
            }
            if (draw)
            {
                draw = false;

                /* Draw score */
//                score_str = number_to_string(score);
//                lives_str = number_to_string(lives);
//                txt = score_str.c_str();
//                al_draw_text(font, black, DISPLAY_WIDTH - 50, DISPLAY_HEIGHT - 5, ALLEGRO_ALIGN_CENTRE, txt);
//                txt = lives_str.c_str();
//                al_draw_text(font, black, 100, DISPLAY_HEIGHT - 5, ALLEGRO_ALIGN_CENTRE, txt);
                /* Next 2 lines draw paddle */
                al_draw_rectangle(x, y, x + paddle_offset_x, y + paddle_offset_y, black, 2);
                al_draw_filled_rectangle(x + 1, y + 1, x + paddle_offset_x - 1, y + paddle_offset_y - 1, white); // draws player
                /* Next 3 lines are for the mouse cursor */
    //            al_draw_circle(cursor_x, cursor_y, cursor_radius, red, 3);
    //            al_draw_line(cursor_x, cursor_y - cursor_radius, cursor_x, cursor_y + cursor_radius, red, 1);
    //            al_draw_line(cursor_x - cursor_radius, cursor_y, cursor_x + cursor_radius, cursor_y, red, 1);

                /* Updates & draws lasers */
                for (unsigned int i = 0; i < laser_list.size(); i++) {
                    Laser* laser = laser_list[i];
                    laser_y = laser->get_y1();
                    laser->update();
                    laser->draw();
                    if (laser_y <= 0) {
                        laser_list.erase(laser_list.begin() + i);
                        delete laser;
                    }
                }
                /* Updates & draws enemies */
                for (unsigned int i = 0; i < enemy_list.size(); i++) {
                    Enemy* enemy = enemy_list[i];
                    enemy->update();
                    enemy->draw();
                }
                /* Updates & draws balls */
                for (unsigned int i = 0; i < ball_list.size(); i++) {
                    Ball* ball = ball_list[i];
                    ball->update();
                    ball->draw();
                }

            al_flip_display();
            al_clear_to_color(green);
        }
    }

    al_flip_display();
    //al_rest(1);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);


    return 0;
}
