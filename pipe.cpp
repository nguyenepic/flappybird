#include"pipe.h"
#include <cstdlib>
#include <vector>
pipe ::pipe(int _x,int _y,SDL_Texture *_texture){
    x=_x;
    y=_y;
    width=120;
    height=300;
    texture=_texture;
      passed = false; // Chưa được vượt qua
     pipeRect.x = x;
     pipeRect.y = y;
     pipeRect.w = width;
     pipeRect.h = height;
}
    // Cập nhật vị trí của ống nước
void pipe::update(int speed) {
    x -= speed; // Di chuyển từ phải sang trái
    pipeRect.x = x;
}
// Kiểm tra xem ống nước có ra khỏi màn hình không
bool pipe::isOffScreen()const {
    return x + width < 0; // Nếu ống ra khỏi cạnh trái màn hình
}
// Vẽ ống nước lên màn hình
void pipe::render(SDL_Renderer* renderer)const {
    SDL_Rect dstRect = { x, y, 120, 300 };
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, (y < 0) ? 180 : 0, NULL, SDL_FLIP_NONE);
}

