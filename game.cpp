#include <iostream>
#include <cstdlib>
#include "game.h"

using namespace std;


game::game(SDL_Texture* birdTexture) : flappy(100, 250, birdTexture) {
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
    double gap = 150; // Khoảng trống giữa hai ống
    double topHeight = rand() % 200 + 50; // Chiều cao ống trên

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
void game::cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture, SDL_Texture* gameover, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(gameover);
    quitSDL(window, renderer); // Gọi hàm quitSDL để giải phóng SDL
}


