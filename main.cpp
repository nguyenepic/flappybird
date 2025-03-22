#include "game.h"
#include <SDL.h>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game tempGame(nullptr);
    window = tempGame.initSDL();
    renderer = tempGame.createRenderer(window);

    // Load hình ảnh
    SDL_Texture *background, *birdTexture, *pipeTexture, *gameover;
    if (!tempGame.loadAllTextures(renderer, background, birdTexture, pipeTexture, gameover)) {
        return -1;
    }
    game flappyGame(birdTexture);
    bool running = true;
    while (running) {
        // Xử lý sự kiện
        flappyGame.handleEvent(running);
        flappyGame.flappy.update();
        flappyGame.flappy.keepInRange();

        // Spawn pipes
        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < flappyGame.SCREEN_WIDTH - 200) {
            flappyGame.spawnpipe(pipeTexture);
        }
        // Cập nhật vị trí ống
        for (auto& p : flappyGame.pipes) {
            p.update(5);
        }
        // Xóa các ống đã ra khỏi màn hình
        flappyGame.pipes.erase(remove_if(flappyGame.pipes.begin(), flappyGame.pipes.end(),
            [](const pipe& p) { return p.isOffScreen(); }), flappyGame.pipes.end());
        // **Kiểm tra Game Over**
        if (flappyGame.checkGameOver(gameover, renderer, running)) {
            if (!running) break;  // Nếu người chơi thoát, kết thúc game
            flappyGame.restartGame(renderer, background, birdTexture, pipeTexture, gameover);
            continue;  // Restart game
        }
        // Vẽ màn hình
        SDL_RenderClear(renderer);
        flappyGame.renderTexture(background, 0, 0, renderer);
        flappyGame.flappy.render(renderer);
        for (const auto& p : flappyGame.pipes) {
            p.render(renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    // Dọn dẹp bộ nhớ
    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover, window, renderer);
    return 0;
}
