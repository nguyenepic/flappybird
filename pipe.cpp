#include"pipe.h"
#include <cstdlib>
#include <vector>
pipe ::pipe(int _x,int _y,SDL_Texture *_texture){
    x=_x;
    y=_y;
    width=50;
    height=300;
    texture=_texture;
      passed = false;
     pipeRect.x = x;
     pipeRect.y = y;
     pipeRect.w = width;
     pipeRect.h = height;
}

void pipe::update(int speed) {
    x -= speed; // Di chuyển từ phải sang trái
    pipeRect.x = x;
}

bool pipe::isOffScreen()const {
    return x + width < 0;
}

void pipe::render(SDL_Renderer* renderer)const {
    SDL_Rect dstRect = { x, y, 50, 300 };
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, (y < 0) ? 180 : 0, NULL, SDL_FLIP_NONE);
}

