

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
    x -= speed; // Di chuyển từ phải sang trái theo tốc độ speed
    pipeRect.x = x;
}

bool pipe::isOffScreen() {
    return x + width < 0;//mép phải
}

void pipe::render(SDL_Renderer* renderer) {//ống dưới hướng lên,ống trên hướng xuống
    SDL_Rect dstRect = { x, y, 50, 300 };//vị trí vẽ ống
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, (y < 0) ? 180 : 0, NULL, SDL_FLIP_NONE);//nếu y<0 thhif xoay 180 độ cho ống phía trên
    //NULL không chỉ định điểm xoay và SDL_FLIP_NONE không lật hình
}
