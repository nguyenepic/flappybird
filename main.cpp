#include "game.h"
#include <SDL.h>
#include <iostream>
#include<algorithm>

int main(int argc, char* argv[]) {
    game flappyGame; // Tạo đối tượng game

    SDL_Window* window = flappyGame.initSDL();

    SDL_Renderer* renderer = flappyGame.createRenderer(window);

    // Load hình ảnh
    SDL_Texture* background = flappyGame.loadTexture("background.png", renderer);
    SDL_Texture* birdTexture = flappyGame.loadTexture("bird.jpg", renderer);
    SDL_Texture* pipeTexture = flappyGame.loadTexture("pipe.jpg", renderer);
    SDL_Texture* Texture = flappyGame.loadTexture("gameover.jpg", renderer);
    // Kiểm tra load hình ảnh có thành công không
    if (!background || !birdTexture || !pipeTexture) return -1;

    bird flappy(100, 250, birdTexture); // Tạo chim
    bool running = true;

    SDL_Event event;

    while (running) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                flappy.jump();
            }
        }

        // Cập nhật game
        flappy.update();

        // Sinh ống nước mới
        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < flappyGame.SCREEN_WIDTH - 200) {
            flappyGame.spawnpipe(pipeTexture);
        }

        // Cập nhật vị trí ống nước
        for (auto& p : flappyGame.pipes) {
            p.update(5);
        }

        // Xóa ống nước đã ra khỏi màn hình
        flappyGame.pipes.erase(remove_if(flappyGame.pipes.begin(), flappyGame.pipes.end(),
            [](const pipe& p) { return p.isOffScreen(); }), flappyGame.pipes.end());

        // Kiểm tra va chạm
        for (const auto& p : flappyGame.pipes) {
            if (flappyGame.checkcollision(flappy, p)) {
                running = false; // Kết thúc game
            }
        }

        // Vẽ màn hình
        SDL_RenderClear(renderer);
        flappyGame.renderTexture(background, 0, 0, renderer);
        flappy.render(renderer);

        for (const auto& p : flappyGame.pipes) {
            p.render(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Giới hạn tốc độ game ~60 FPS
    }

    // Giải phóng tài nguyên
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    flappyGame.quitSDL(window, renderer);

    return 0;
}



