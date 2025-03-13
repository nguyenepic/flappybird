#ifndef GAME_H
#define GAME_H
#include "bird.h"
#include "pipe.h"
#include<SDL.h>
#include<SDL_image.h>
#include<vector>
struct game{
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "flappy bird";
void logErrorAndExit(const char* msg, const char* error);
SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE);
SDL_Renderer* createRenderer(SDL_Window* window);
std::vector<pipe> pipes; // Lưu danh sách ống nước
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Rect srcplayer,destplayer;
SDL_Texture *background;
void renderTexture(SDL_Texture *texture, int x, int y, SDL_Renderer* renderer);
SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer);
void spawnpipe(SDL_Texture* pipeTexture);
void update();
void keyboardinput();
bool checkcollision( const pipe& p);
    game();  // Khai báo constructor
    bool running;
    bool isrunning() { return running; }
bird flappy;
};
#endif // GAME_H
