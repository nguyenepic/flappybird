#include "game.h"
#include <SDL.h>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game tempGame;

    window = tempGame.initSDL();
    renderer = tempGame.createRenderer(window);
    Mix_Chunk* hitSound = NULL;
    Mix_Chunk* flapSound = NULL;
    Mix_Music* backgroundmusic = NULL;

    // Load hình ảnh
    SDL_Texture *background, *birdTexture, *pipeTexture, *gameover;
    if (!tempGame.loadAllTextures(renderer, background, birdTexture, pipeTexture, gameover)) {
        return -1;
    }
    if (!tempGame.loadSounds()) {
    SDL_Log("ERROR: Failed to load sounds!");
    return -1;
}

// Lấy âm thanh từ `tempGame`
flapSound = tempGame.getFlapSound();
hitSound = tempGame.getHitSound();
backgroundmusic = tempGame.getBackgroundMusic();


Mix_Volume(-1, MIX_MAX_VOLUME);
Mix_VolumeMusic(MIX_MAX_VOLUME);
SDL_Log("🎵 Testing sound before game loop...");

// Phát thử flapSound

    Mix_PlayChannel(-1, flapSound, 0);

// Phát thử background music

    Mix_PlayMusic(backgroundmusic, -1);

    game flappyGame; // Dùng constructor mặc định, tránh ghi đè dữ liệu
    flappyGame = tempGame; // Copy tempGame vào flappyGame
    flappyGame.flappy = bird(100, 250, birdTexture); // Đảm bảo flappy có texture

    bool running = true;
    while (running) {

        // Xử lý sự kiện
        flappyGame.handleEvent(running);
        flappyGame.flappy.update();
        flappyGame.flappy.keepInRange();

        // Spawn pipes
        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < SCREEN_WIDTH - 200) {
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
    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover,flapSound,hitSound,backgroundmusic, window, renderer);
    return 0;
}
