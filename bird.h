#ifndef BIRD_H
#define BIRD_H
#include<SDL.h>
#include<SDL_image.h>
struct bird{
int x,y;
double gravity;
double speed;
double jumpstrength;
SDL_Texture *texture;
SDL_Rect birdRect; // Định vị vị trí vẽ chim
bird(int _x,int _y,SDL_Texture* _texture);
    // Constructor mặc định
    bird() {
        x = 0;
        y = 0;
        gravity = 0.5;
        speed = 0;
        jumpstrength = 10;
        texture = nullptr;
        birdRect = {0, 0, 50, 50};
    }
void update();
void falling();
void jump();
bool keepInRange();
void render(SDL_Renderer* renderer);
};
#endif // BIRD_H
