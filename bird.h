#ifndef BIRD_H
#define BIRD_H

#include<SDL.h>
#include<SDL_image.h>
struct bird{
double x,y;
double gravity;
double speed;
double jumpstrength;
SDL_Texture *texture;
Bird(int _x,int _y,SDL_Texture *bird);
void falling();
void jump();
void render(SDL_Renderer *render);
};

#endif // BIRD_H
