#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>
#include <SDL_image.h>
const int TOTAL_FRAMES = 4;
const int ANIMATION_SPEED = 100;//mỗi 0,1 giây hiển thị 1 frame
struct bird {
    int x, y;
    double gravity;
    double speed;
    double jumpstrength;
    SDL_Texture* texture;
    SDL_Rect birdRect;

    int frameIndex;       // Frame hiện tại
    Uint32 lastTime;      // Thời gian cập nhật frame gần nhất

    bird(int _x, int _y, SDL_Texture* _texture);
    bird() {
        x = 0;
        y = 0;
        gravity = 0.5;
        speed = 0;
        jumpstrength = 10;
        texture = nullptr;
        frameIndex = 0;
        lastTime = SDL_GetTicks();
    }
    void update();
    void falling();
    void jump();
    bool keepInRange();
    void render(SDL_Renderer* renderer);

    void updateAnimation();
    void renderAnimation(SDL_Renderer* renderer);
};

#endif // BIRD_H
