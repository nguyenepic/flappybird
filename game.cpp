#include <iostream>
#include <cstdlib>
#include<algorithm>
#include "game.h"
using namespace std;
game::game(SDL_Texture* birdTexture)
    : flappy(100, 250, birdTexture), flapSound(nullptr), hitSound(nullptr), backgroundmusic(nullptr) {
    running = true;
}
void game::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
    exit(EXIT_FAILURE);
}
SDL_Window* game::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit("SDL_image error:", IMG_GetError());

    return window;
}
SDL_Renderer* game::createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}
bool game::loadAllTextures(SDL_Renderer* renderer, SDL_Texture*& background, SDL_Texture*& birdTexture, SDL_Texture*& pipeTexture, SDL_Texture*& gameover) {
    background = loadTexture("background.jpg", renderer);
    birdTexture = loadTexture("bird.png", renderer);
    pipeTexture = loadTexture("pipe.png", renderer);
    gameover = loadTexture("gameover.png", renderer);

    if (!background || !birdTexture || !pipeTexture || !gameover) {
        SDL_Log("Failed to load images!");
        return false;
    }
    return true;
}
bool game::loadSounds() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("❌ SDL_mixer could not initialize! Error: %s", Mix_GetError());
        return false;
    }

    flapSound = Mix_LoadWAV("flapSound.wav");

if (!flapSound) {
    SDL_Log("❌ Failed to load flapSound.wav! SDL_mixer Error: %s", Mix_GetError());
    return false;  // Nếu âm thanh không load được, báo lỗi ngay
}

    if (!flapSound) {
        SDL_Log("❌ Failed to load flapSound.wav! SDL_mixer Error: %s", Mix_GetError());
    } else {
        SDL_Log("✅ flapSound loaded successfully!");
    }

    hitSound = Mix_LoadWAV("hitSound.wav");
    if (!hitSound) {
        SDL_Log("❌ Failed to load hitSound.wav! SDL_mixer Error: %s", Mix_GetError());
    }

    backgroundmusic = Mix_LoadMUS("backgroundmusic.mp3");
    if (!backgroundmusic) {
        SDL_Log("❌ Failed to load backgroundmusic.mp3! SDL_mixer Error: %s", Mix_GetError());
    }

    return flapSound && hitSound && backgroundmusic;
}


Mix_Chunk* game::getFlapSound() const { return flapSound; }
Mix_Chunk* game::getHitSound() const { return hitSound; }
Mix_Music* game::getBackgroundMusic() const { return backgroundmusic; }


void game::playSound(Mix_Chunk* sound) {
    if (sound) {
        if (Mix_PlayChannel(-1, sound, 0) == -1) {
            SDL_Log("❌ Error playing sound: %s", Mix_GetError());
        } else {
            SDL_Log("✅ Sound played successfully!");
        }
    } else {
        SDL_Log("❌ Sound is NULL, cannot play!");
    }
}


void game::quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}
void game::handleEvent(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            flappy.jump();

            if (flapSound) {
                SDL_Log("🔊 Playing flapSound...");
                if (Mix_PlayChannel(-1, flapSound, 0) == -1) {
                    SDL_Log("❌ Error playing flapSound: %s", Mix_GetError());
                }
            } else {
                SDL_Log("❌ flapSound is NULL!");
            }
        }
    }
}


void game::renderTexture(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer) {
    SDL_Rect dest = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

SDL_Texture* game::loadTexture(const char* filename, SDL_Renderer* renderer) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL)
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

    return texture;
}
void game::spawnpipe(SDL_Texture* pipeTexture) {
    double pipeX = SCREEN_WIDTH;
    double gap = 210; // Khoảng trống giữa hai ống
    double topHeight = rand() % 150 + 100; // Chiều cao ống trên

    // Ống trên
    pipes.push_back(pipe(pipeX, topHeight - 300, pipeTexture));

    // Ống dưới
    pipes.push_back(pipe(pipeX, topHeight + gap, pipeTexture));
}
void game::update() {
    srcplayer = {0, 0, 80, 60};
    destplayer = {20, 20, 60, 80};
}
bool game::checkcollision(const bird& b, const pipe& p) const {
    return SDL_HasIntersection(&b.birdRect, &p.pipeRect);
}
bool game::checkGameOver(SDL_Texture* gameover, SDL_Renderer* renderer, bool& running) {
    // Kiểm tra va chạm với ống hoặc rơi khỏi màn hình
    for (const auto& p : pipes) {
        if (checkcollision(flappy, p) || flappy.birdRect.y > SCREEN_HEIGHT) {
                playSound(hitSound);
            // Hiển thị Game Over
            SDL_RenderCopy(renderer, gameover, NULL, NULL);
            SDL_RenderPresent(renderer);

            // Chờ người chơi nhấn phím
            SDL_Event event;
            while (true) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        return true;
                    }
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;  // Thoát game
                            return true;
                        } else {
                            return true;  // Nhấn phím khác để restart
                        }
                    }
                }
                SDL_Delay(100);
            }
        }
    }
    return false;  // Chưa game over
}
void game::restartGame(SDL_Renderer* renderer, SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture, SDL_Texture* gameover) {
    // Reset vị trí chim
    flappy = bird(100, 250, birdTexture);

    // Xóa tất cả ống
    pipes.clear();

    // Bắt đầu lại vòng lặp
    running = true;
}
void game::cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture, SDL_Texture* gameover,
                   Mix_Chunk* flapSound, Mix_Chunk* hitSound, Mix_Music* backgroundmusic,
                   SDL_Window* window, SDL_Renderer* renderer) {
    // Giải phóng các texture
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(gameover);

    // Giải phóng âm thanh
    Mix_FreeChunk(flapSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(backgroundmusic);
    flapSound = nullptr;
    Mix_CloseAudio();

    // Giải phóng SDL
    quitSDL(window, renderer);
}


