#include "game.h"
#include <SDL_ttf.h>
#include <cstdlib>
#include <algorithm>
#include<string>
using namespace std;
game::game(SDL_Texture* birdTexture)
    : flappy(100, 250, birdTexture) {
    running = true;
}

void game::handleEvent(bool& running, Mix_Chunk* flapSound) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            flappy.jump();
            audio.playSound(flapSound);
        }
    }
}

void game::spawnpipe(SDL_Texture* pipeTexture) {
    double pipeX = SCREEN_WIDTH;
    double gap = 210;
    double topHeight = rand() % 150 + 100;

    pipes.push_back(pipe(pipeX, topHeight - 300, pipeTexture));
    pipes.push_back(pipe(pipeX, topHeight + gap, pipeTexture));
}

void game::renderScore(SDL_Renderer* renderer, int score) {
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        SDL_Log("ERROR: Failed to load font: %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255};
    string scoreText = "Score: " + std::to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        SDL_Log("ERROR: Failed to create text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("ERROR: Failed to create text texture: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

bool game::checkcollision(const bird& b, const pipe& p) const {
    return SDL_HasIntersection(&b.birdRect, &p.pipeRect);
}

bool game::checkGameOver(SDL_Renderer* renderer, SDL_Texture* gameover,SDL_Texture *background,int score, Mix_Chunk* hitSound, bool& running) {
    for (const auto& p : pipes) {
        if (checkcollision(flappy, p) || flappy.birdRect.y > SCREEN_HEIGHT) {
            audio.playSound(hitSound);

            // Hiển thị lại toàn bộ khung hình trước khi vẽ Game Over
            SDL_RenderClear(renderer);

// Vẽ lại background trước
SDL_RenderCopy(renderer, background, nullptr, nullptr);

// Vẽ lại các vật thể khác
for (const auto& p : pipes) {
    p.render(renderer);
}

flappy.render(renderer);
renderScore(renderer, score); // Hiển thị điểm số

// Hiển thị Game Over ở giữa màn hình
            SDL_Rect goRect = { (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 200) / 2, 400, 200 };
            SDL_RenderCopy(renderer, gameover, nullptr, &goRect);
             SDL_RenderPresent(renderer);


            // Đợi 1.5 giây trước khi cho phép restart
            SDL_Delay(100);

            // Chờ phím nhấn từ người chơi
            SDL_Event event;
            while (true) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        return true;
                    }
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;
                            return true;
                        } else if (event.key.keysym.sym == SDLK_SPACE) {
                            return true; // Nhấn SPACE để restart
                        }
                    }
                }
                SDL_Delay(100);
            }
        }
    }
    return false;
}


void game::restartGame(SDL_Texture* birdTexture) {
    flappy = bird(100, 250, birdTexture);
    pipes.clear();
    running = true;
    SDL_Log("Game restarted.");
}

void game::cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture,
                   SDL_Texture* gameover, Mix_Chunk* flapSound, Mix_Chunk* hitSound,
                   Mix_Music* backgroundMusic) {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(gameover);

    Mix_FreeChunk(flapSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(backgroundMusic);

    Mix_CloseAudio();
    graphic.cleanup();
    SDL_Log("Game resources cleaned up.");
}
