#include "game.h"
#include "bird.h"

bird ::bird(int _x,int _y,SDL_Texture *bird){
    x=_x;
    y=_y;
    speed=0;
    gravity=1;
    jumpstrength=-10;
    texture=bird;

    birdRect.x = x;
    birdRect.y = y;

    birdRect.w = 70;
    birdRect.h = 70;
}
void bird::update() {
    falling(); // Gọi hàm falling() để áp dụng trọng lực
}
void bird::falling(){
    speed+=gravity;
    y+=speed;
    // Cập nhật lại vị trí birdRect mà không ép kiểu
    birdRect.x = x;
    birdRect.y = y;
}
void bird ::jump(){
    speed=jumpstrength;
}
void bird::render(SDL_Renderer* renderer) {
    // Kiểm tra nếu texture tồn tại trước khi vẽ
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &birdRect);
    }
}
bool bird::keepInRange() {
    if (birdRect.y < 0) {
        birdRect.y = 0;  // Không cho bay quá cao
        return false;    // Trả về false để báo là chạm trần
    }
    return true; // Chim vẫn trong màn hình (hoặc có thể rơi xuống dưới)
}


