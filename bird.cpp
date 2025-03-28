#include "game.h"
#include "bird.h"

int FRAME_WIDTH;  // Khai báo biến toàn cục
int FRAME_HEIGHT; // Khai báo biến toàn cục
// Constructor khởi tạo bird với sprite sheet
bird::bird(int _x, int _y, SDL_Texture *birdTexture) {
    x = _x;
    y = _y;
    speed = 0;
    gravity = 1;
    jumpstrength = -10;
    texture = birdTexture; // Sửa lỗi gán sai biến

    // Lấy kích thước thật của texture
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    FRAME_WIDTH = texW / TOTAL_FRAMES;  // Tự động tính chiều rộng frame
    FRAME_HEIGHT = texH; // Giữ nguyên chiều cao frame

    birdRect.x = x;
    birdRect.y = y;
    birdRect.w = FRAME_WIDTH/3;
    birdRect.h = FRAME_HEIGHT/3;

    // Khởi tạo animation
    frameIndex = 0;
    lastTime = SDL_GetTicks();
}

// Cập nhật trạng thái chim
void bird::update() {
    falling(); // Gọi hàm falling() để áp dụng trọng lực
    updateAnimation(); // Cập nhật animation
}

// Chim rơi xuống do trọng lực
void bird::falling() {
    speed += gravity;
    y += speed;

    // Giữ chim trong màn hình
    if (y + FRAME_HEIGHT > SCREEN_HEIGHT) {
        y = SCREEN_HEIGHT - FRAME_HEIGHT;
        speed = 0;
    }

    birdRect.x = x;
    birdRect.y = y;
}

// Chim nhảy lên
void bird::jump() {
    speed = jumpstrength;
}

// Giữ chim trong màn hình
bool bird::keepInRange() {
    if (birdRect.y < 0) {
        birdRect.y = 0;
        speed = 0;
        return false; // Chim chạm trần
    }
    return true;
}

// Cập nhật animation (chuyển frame)
void bird::updateAnimation() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= ANIMATION_SPEED) { // Sửa lỗi logic
        frameIndex = (frameIndex + 1) % TOTAL_FRAMES;
        lastTime = currentTime;
    }
}

// Vẽ chim với animation
void bird::renderAnimation(SDL_Renderer* renderer) {
    SDL_Rect srcRect = {frameIndex * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT};
    SDL_RenderCopy(renderer, texture, &srcRect, &birdRect);
}

// Vẽ chim (không animation)
void bird::render(SDL_Renderer* renderer) {
    if (texture) {
        renderAnimation(renderer); // Gọi hàm vẽ animation thay vì vẽ cố định
    }
}
