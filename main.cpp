#include "game.h"
#include <SDL.h>
#include <iostream>
#include<algorithm>
using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;

    game flappyGame(nullptr);  // Khởi tạo game trước khi có texture

    window = flappyGame.initSDL();
    renderer = flappyGame.createRenderer(window);

    SDL_Texture *background, *birdTexture, *pipeTexture, *gameover;
    if (!flappyGame.loadAllTextures(renderer, background, birdTexture, pipeTexture, gameover)) {
        return -1;
    }

    if (!flappyGame.loadSounds()) {
        SDL_Log("ERROR: Failed to load sounds!");
        return -1;
    }

    Mix_Volume(-1, MIX_MAX_VOLUME);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_PlayMusic(flappyGame.getBackgroundMusic(), -1);

    // Gán texture cho chim
    flappyGame.restartGame(renderer,background,birdTexture,pipeTexture,gameover);

    bool running = true;
    while (running) {
        flappyGame.handleEvent(running);
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

        if (flappyGame.checkGameOver(gameover, renderer, running)) {
            if (!running) break;
            flappyGame.restartGame(renderer,background,birdTexture,pipeTexture,gameover);
            continue;
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

    flappyGame.cleanup(background, birdTexture, pipeTexture, gameover,
                       flappyGame.getFlapSound(), flappyGame.getHitSound(),
                       flappyGame.getBackgroundMusic(), window, renderer);

    return 0;
}
