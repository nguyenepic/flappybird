#ifndef GAME_H
#define GAME_H

#include "bird.h"
#include "pipe.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
struct game {

    const char* WINDOW_TITLE = "flappy bird";
    Mix_Chunk* flapSound;
    Mix_Chunk* hitSound;
    Mix_Music* backgroundmusic;
    Mix_Chunk* getFlapSound() const;
    Mix_Chunk* getHitSound() const;
    Mix_Music* getBackgroundMusic() const;


      game();  // Thêm constructor mặc định
    game(SDL_Texture* birdTexture);  // Constructor có tham số
    bool loadAllTextures(SDL_Renderer* renderer, SDL_Texture*& background, SDL_Texture*& birdTexture, SDL_Texture*& pipeTexture, SDL_Texture*& gameover);
     void loadSound();
     void playBackgroundMusic(Mix_Music* music);
    void playSound(Mix_Chunk* sound);
    bool loadSounds();
    bool running;
    bool isrunning() { return running; }
    void logErrorAndExit(const char* msg, const char* error);
    SDL_Window* initSDL();
    SDL_Renderer* createRenderer(SDL_Window* window);
    void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
    void waitUntilKeyPressed();
    void handleEvent(bool& running);
    std::vector<pipe> pipes;
    void spawnpipe(SDL_Texture* pipeTexture);
    void update();
    bool checkcollision(const bird& b, const pipe& p) const;
    bool checkGameOver(SDL_Texture* gameover, SDL_Renderer* renderer, bool& running);

    SDL_Rect srcplayer, destplayer;
    SDL_Texture* background;
    void renderTexture(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer);
    SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);
    void cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture, SDL_Texture* gameover,
                   Mix_Chunk* flapSound, Mix_Chunk* hitSound, Mix_Music* backgroundmusic,
                   SDL_Window* window, SDL_Renderer* renderer);
    void restartGame(SDL_Renderer* renderer, SDL_Texture* background, SDL_Texture* birdTexture,
                 SDL_Texture* pipeTexture, SDL_Texture* gameover);

    bird flappy; // Bird là thành viên của game
};
#endif // GAME_H
