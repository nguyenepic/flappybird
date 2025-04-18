
#ifndef PIPE_H
#define PIPE_H

#include<SDL.h>
#include<SDL_image.h>
#include<vector>
struct pipe{
    double x,y;
    double width,height;
    SDL_Texture *texture;
    SDL_Rect pipeRect;
    bool passed;
    void update(int speed);
    void render(SDL_Renderer* renderer) ;

    bool isOffScreen() ;
    pipe(int x, int y, SDL_Texture* texture);
};

#endif // PIPE_H
