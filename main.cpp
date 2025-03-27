#include "game.h"
#include "audio.h"
#include <SDL.h>
#include <iostream>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game flappyGame(nullptr);
    Audio audio;

    window = flappyGame.initSDL();
    renderer = flappyGame.createRenderer(window);

    SDL_Texture *background, *birdTexture, *pipeTexture, *gameover;
    if (!flappyGame.loadAllTextures(renderer, background, birdTexture, pipeTexture, gameover)) {
        return -1;
    }

    if (!audio.loadSounds()) {  // Sử dụng audio thay vì flappyGame.loadSounds()
        SDL_Log("ERROR: Failed to load sounds!");
        return -1;
    }

    Mix_Volume(-1, MIX_MAX_VOLUME);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_PlayMusic(audio.getBackgroundMusic(), -1);

    int score = 0;
    flappyGame.restartGame(renderer, background, birdTexture, pipeTexture, gameover);
    bool running = true;

    while (running) {
        flappyGame.handleEvent(running,audio.flapSound);

        flappyGame.flappy.update();
        flappyGame.flappy.keepInRange();

        if (flappyGame.pipes.empty() || flappyGame.pipes.back().x < SCREEN_WIDTH - 200) {
            flappyGame.spawnpipe(pipeTexture);
        }

        for (auto& p : flappyGame.pipes) {
            p.update(5);
        }

        flappyGame.pipes.erase(remove_if(flappyGame.pipes.begin(), flappyGame.pipes.end(),
                                         [](const pipe& p) { return p.isOffScreen(); }), flappyGame.pipes.end());

        if (flappyGame.checkGameOver(gameover,audio.hitSound, renderer, running)) {
            if (!running) break;
            score = 0;
            flappyGame.restartGame(renderer, background, birdTexture, pipeTexture, gameover);
            continue;
        }

        // Cập nhật điểm nếu chim vượt qua ống
        for (auto& p : flappyGame.pipes) {
            if (!p.passed && p.x + p.width < flappyGame.flappy.x && p.y > 0) {
                score++;
                p.passed = true;
            }
        }

        SDL_RenderClear(renderer);
        flappyGame.renderTexture(background, 0, 0, renderer);
        flappyGame.flappy.render(renderer);
        for (const auto& p : flappyGame.pipes) {
            p.render(renderer);
        }
        flappyGame.renderScore(renderer, score);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_Quit();
    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover,
                       audio.getFlapSound(), audio.getHitSound(),
                       audio.getBackgroundMusic(), window, renderer);
    return 0;
}
