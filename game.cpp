
#include "game.h"
#include <SDL_ttf.h>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

game::game(SDL_Texture* birdTexture)
    : flappy(50, 100, birdTexture) {
    running = true;
}

void game::handleEvent(bool& running, Mix_Chunk* flapSound,SDL_Renderer *renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                flappy.jump();
                audio.playSound(flapSound);
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                showMenu(running,renderer);
            }
        }
    }
}
void game::showMenu(bool& running,SDL_Renderer* renderer) {
    bool inMenu = true;
    int selectedOption = 0;
    const string menuText[] = {"Continue", "Restart", "Quit"};
    int menuSize = 3;

    while (inMenu) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    selectedOption = (selectedOption + 1) % menuSize;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    selectedOption = (selectedOption - 1 + menuSize) % menuSize;
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (selectedOption == 0) { // Continue
                        inMenu = false;
                    } else if (selectedOption == 1) { // Restart
                        restartGame(flappy.texture);
                        inMenu = false;
                    } else if (selectedOption == 2) { // Quit
                        running = false;
                        inMenu = false;
                    }
                }
            }
        }
        renderMenu(selectedOption,renderer);
    }
}
void game::renderMenu(int selectedOption,SDL_Renderer* renderer) {
    SDL_Color textColor = {255, 255, 255};
    SDL_Color highlightColor = {255, 0, 0};
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) return;

    const string menuText[] = {"Continue", "Restart", "Quit"};
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 30};

    for (int i = 0; i < 3; i++) {
        SDL_Color color = (i == selectedOption) ? highlightColor : textColor;
        SDL_Surface* surface = TTF_RenderText_Solid(font, menuText[i].c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
        textRect.y += 40;
    }
    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);
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

    SDL_Color textColor = {255, 255, 255}; // Trắng
    string scoreText = "Score: " + to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        SDL_Log("ERROR: Failed to create text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Giải phóng ngay sau khi tạo texture

    if (!textTexture) {
        SDL_Log("ERROR: Failed to create text texture: %s", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture); // Giải phóng bộ nhớ sau khi render
    TTF_CloseFont(font);
}

bool game::checkcollision(const bird& b, const pipe& p) const {
    return SDL_HasIntersection(&b.birdRect, &p.pipeRect);
}

bool game::checkGameOver(SDL_Renderer* renderer, SDL_Texture* gameover, SDL_Texture* background, int score, Mix_Chunk* hitSound, bool& running) {
    for (const auto& p : pipes) {
        if (checkcollision(flappy, p) || flappy.birdRect.y > SCREEN_HEIGHT) {
            audio.playSound(hitSound);

            SDL_RenderCopy(renderer, background, nullptr, nullptr);

            for (const auto& p : pipes) {
                p.render(renderer);
            }

            flappy.render(renderer);
            renderScore(renderer, score); // Hiển thị điểm số

            SDL_Rect goRect = { (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 200) / 2, 400, 200 };
            SDL_RenderCopy(renderer, gameover, nullptr, &goRect);
            SDL_RenderPresent(renderer);

            SDL_Delay(100); // Delay ngắn trước khi nhận input

            // Chờ phím nhấn từ người chơi (Timeout tránh treo game)
            Uint32 startTime = SDL_GetTicks();
            SDL_Event event;

            while (SDL_GetTicks() - startTime < 5000) { // 5 giây timeout
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
            }
            running = false;
            return true;
        }
    }
    return false;
}

void game::restartGame(SDL_Texture* birdTexture) {
    flappy = bird(50, 250, birdTexture);
    pipes.clear();
    running = true;
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
}
