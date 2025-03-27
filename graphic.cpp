#include "graphic.h"
#include <iostream>
#include <SDL_ttf.h>

Graphic::Graphic() : window(nullptr), renderer(nullptr) {}

Graphic::~Graphic() {
    cleanup();
}

void Graphic::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

SDL_Window* Graphic::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
        logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit("SDL_image error:", IMG_GetError());
    if (TTF_Init() == -1)
    logErrorAndExit("SDL_ttf Init", TTF_GetError());
    return window;
}



SDL_Renderer* Graphic::createRenderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

SDL_Texture* Graphic::loadTexture(const char* filename) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture)
        SDL_Log("Load texture %s failed: %s", filename, IMG_GetError());
    return texture;
}

void Graphic::renderTexture(SDL_Texture* texture, int x, int y) {
    SDL_Rect dest = {x, y, 0, 0};
    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
}

void Graphic::cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
