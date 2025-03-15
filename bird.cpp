#include"bird.h"
bird ::bird(double _x,double _y,SDL_Texture *bird){
    x=_x;
    y=_y;
    speed=0;
    gravity=1;
    jumpstrength=-12;
    texture=bird;
    // Khởi tạo birdRect nhưng không ép kiểu
    birdRect.x = static_cast<int>(x);
    birdRect.y = static_cast<int>(y);

    birdRect.w = 40;
    birdRect.h = 40;
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
