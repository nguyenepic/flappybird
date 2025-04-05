#include "game.h"
#include "bird.h"

int FRAME_WIDTH;
int FRAME_HEIGHT;
bird::bird(int _x, int _y, SDL_Texture *birdTexture) {
    x = _x;
    y = _y;
    speed = 0;
    gravity = 1;
    jumpstrength = -10;
    texture = birdTexture;

    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    FRAME_WIDTH = texW / TOTAL_FRAMES;  //  tính chiều rộng frame
    FRAME_HEIGHT = texH; // Giữ nguyên chiều cao frame

    birdRect.x = x;
    birdRect.y = y;
    birdRect.w = FRAME_WIDTH/3;
    birdRect.h = FRAME_HEIGHT/3;

    frameIndex = 0;
    lastTime = SDL_GetTicks();
}

void bird::update() {
    falling(); //
    updateAnimation();
}

void bird::falling() {
    speed += gravity;//speed tăng dần do trọng lực,khiến chim ngày càng rơi nhanh hơn
    y += speed;//cập nhật vị trí chim theo tốc độ rơi
    if (y > SCREEN_HEIGHT) {
    birdRect.y = SCREEN_HEIGHT + 100; // Đưa ra ngoài màn hình nếu chim rơi ra ngoài màn hình
}

    birdRect.x = x;
    birdRect.y = y;
}

void bird::jump() {
    speed = jumpstrength;//tốc độ rơi sẽ bị giảm về jumpsrength làm chim di chuyển lên trên
}

bool bird::keepInRange() {
    if (birdRect.y < 0) {
        birdRect.y = 0;
        speed=0;//ngăn chim đi lên
        return false;
    }
    return true;
}

void bird::updateAnimation() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= ANIMATION_SPEED) {//kiểm tra đến lúc chuyển frame chưa
        frameIndex = (frameIndex + 1) % TOTAL_FRAMES;//vòng lặp các frame, chuyển về frame đầu tiên khi đến frame cuối
        lastTime = currentTime;
    }
}

void bird::renderAnimation(SDL_Renderer* renderer) {
    SDL_Rect srcRect = {frameIndex * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT};//frameindex*frame_width :cắt ra phần tương ứng với frame hiện tại,y=0 do frame xếp ngang
    SDL_RenderCopy(renderer, texture, &srcRect, &birdRect);
}


