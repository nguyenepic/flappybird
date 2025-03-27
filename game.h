
#ifndef GAME_H
#define GAME_H
#include "graphic.h"
#include "audio.h"
#include "bird.h"
#include "pipe.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
struct game {
    const char* WINDOW_TITLE = "flappy bird";
      Audio audio; // Thêm biến này để gọi các hàm trong Audio
      game();  // Thêm constructor mặc định
    game(SDL_Texture* birdTexture);  // Constructor có tham số
    bool loadAllTextures(SDL_Renderer* renderer, SDL_Texture*& background, SDL_Texture*& birdTexture, SDL_Texture*& pipeTexture, SDL_Texture*& gameover);

     void playBackgroundMusic(Mix_Music* music);


    bool running;
    bool isrunning() { return running; }
    void logErrorAndExit(const char* msg, const char* error);
    SDL_Window* initSDL();
    SDL_Renderer* createRenderer(SDL_Window* window);
    void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
    void waitUntilKeyPressed();
    void handleEvent(bool& running,Mix_Chunk* flapSound);
    std::vector<pipe> pipes;
    void spawnpipe(SDL_Texture* pipeTexture);
    void renderScore(SDL_Renderer* renderer, int score);
    void update();
    bool checkcollision(const bird& b, const pipe& p) const;
    bool checkGameOver(SDL_Texture* gameover,Mix_Chunk* hitSound, SDL_Renderer* renderer, bool& running);

    SDL_Rect srcplayer, destplayer;
    SDL_Texture* background;
    void renderTexture(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer);
    SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);
  void cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture,
             SDL_Texture* gameover, Mix_Chunk* flapSound, Mix_Chunk* hitSound,
             Mix_Music* backgroundMusic, SDL_Window* window, SDL_Renderer* renderer);


    void restartGame(SDL_Renderer* renderer, SDL_Texture* background, SDL_Texture* birdTexture,
                 SDL_Texture* pipeTexture, SDL_Texture* gameover);

    bird flappy; // Bird là thành viên của game
};
#endif // GAME_H
