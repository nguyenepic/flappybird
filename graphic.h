#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
struct Graphic {
    SDL_Window* window;
    SDL_Renderer* renderer;

    Graphic();
    ~Graphic();

    void logErrorAndExit(const char* msg, const char* error);
    SDL_Window* initSDL();
    SDL_Renderer* createRenderer(SDL_Window* window);
    SDL_Texture* loadTexture(const char* filename);
    void renderTexture(SDL_Texture* texture, int x, int y);
    void cleanup();
};

#endif // GRAPHIC_H
