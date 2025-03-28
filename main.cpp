#include "game.h"
#include "audio.h"
#include "graphic.h"
#include <SDL.h>
#include <algorithm>

int main(int argc, char* argv[]) {
    Graphic graphic;
    Audio audio;

    // Khởi tạo SDL và tạo cửa sổ + renderer
    graphic.initSDL();
    graphic.createRenderer(graphic.window);

    // Load các texture
    SDL_Texture *background = graphic.loadTexture("background.jpg");
    SDL_Texture *birdTexture = graphic.loadTexture("bird.png"); // Sprite sheet
    SDL_Texture *pipeTexture = graphic.loadTexture("pipe.png");
    SDL_Texture *gameover = graphic.loadTexture("gameover.png");

    if (!background || !birdTexture || !pipeTexture || !gameover) {
        SDL_Log("ERROR: Failed to load textures!");
        return -1;
    }

    // Load âm thanh
    if (!audio.loadSounds()) {
        SDL_Log("ERROR: Failed to load sounds!");
        return -1;
    }
    Mix_Volume(-1, MIX_MAX_VOLUME);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_PlayMusic(audio.getBackgroundMusic(), -1);

    game flappyGame(birdTexture);
    flappyGame.restartGame(birdTexture);
    int score = 0;
    bool running = true;

    while (running) {
        flappyGame.handleEvent(running, audio.getFlapSound(),graphic.renderer);

        if (!running) break; // Nếu menu chọn Quit, thoát game ngay

        flappyGame.flappy.update();
        flappyGame.flappy.keepInRange();

        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < SCREEN_WIDTH - 200) {
            flappyGame.spawnpipe(pipeTexture);
        }

        for (auto& p : flappyGame.pipes) {
            p.update(5);
        }

        flappyGame.pipes.erase(std::remove_if(flappyGame.pipes.begin(), flappyGame.pipes.end(),
                                              [](const pipe& p) { return p.isOffScreen(); }),
                               flappyGame.pipes.end());

        if (flappyGame.checkGameOver(graphic.renderer, gameover, background, score, audio.getHitSound(), running)) {
            score = 0;
            flappyGame.restartGame(birdTexture);
            continue;
        }

        for (auto& p : flappyGame.pipes) {
            if (!p.passed && p.x + p.width < flappyGame.flappy.x && p.y > 0) {
                score++;
                p.passed = true;
            }
        }

        SDL_RenderClear(graphic.renderer);
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(graphic.renderer, background, nullptr, &bgRect);

        // Render bird với animation
        flappyGame.flappy.renderAnimation(graphic.renderer);

        for (const auto& p : flappyGame.pipes) {
            p.render(graphic.renderer);
        }

        flappyGame.renderScore(graphic.renderer, score);
        SDL_RenderPresent(graphic.renderer);
        SDL_Delay(16); // Khoảng 60 FPS
    }

    TTF_Quit();
    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover,
                       audio.getFlapSound(), audio.getHitSound(),
                       audio.getBackgroundMusic());

    return 0;
}
