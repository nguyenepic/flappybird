#include "game.h"
#include <SDL.h>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game tempGame(nullptr);  // Dùng tạm một object để gọi `loadTexture`
    window = tempGame.initSDL();
    renderer = tempGame.createRenderer(window);
    // Load hình ảnh
    SDL_Texture* background = tempGame.loadTexture("background.jpg", renderer);
    SDL_Texture* birdTexture = tempGame.loadTexture("bird.png", renderer);
    SDL_Texture* pipeTexture = tempGame.loadTexture("pipe.jpg", renderer);
    SDL_Texture* gameover = tempGame.loadTexture("gameover.jpg", renderer);
    // Kiểm tra load hình ảnh có thành công không
    if (!background || !birdTexture || !pipeTexture) {
        cout << "Failed to load images!" << endl;
        return -1;
    }
    // Chỉ tạo game sau khi `birdTexture` đã load xong
    game flappyGame(birdTexture);
    bool running = true;
    SDL_Event event;
    while (running) {
       flappyGame.handleEvent(running);
        flappyGame.flappy.update();
        flappyGame.flappy.keepInRange();  // Đảm bảo chim luôn trong màn hình
        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < flappyGame.SCREEN_WIDTH - 200) {
            flappyGame.spawnpipe(pipeTexture);
        }
        for (auto& p : flappyGame.pipes) {
            p.update(5);
        }
        flappyGame.pipes.erase(remove_if(flappyGame.pipes.begin(), flappyGame.pipes.end(),
            [](const pipe& p) { return p.isOffScreen(); }), flappyGame.pipes.end());

        for (const auto& p : flappyGame.pipes) {
           if (flappyGame.checkGameOver(gameover, renderer, running)) {
           break;
}

        }
        SDL_RenderClear(renderer);
        flappyGame.renderTexture(background, 0, 0, renderer);
        flappyGame.flappy.render(renderer);

        for (const auto& p : flappyGame.pipes) {
            p.render(renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    // Giải phóng tài nguyên
    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover, window, renderer);
}
