

#ifndef GAME_H
#define GAME_H

#include "graphic.h"
#include "audio.h"
#include "bird.h"
#include<SDL_ttf.h>
#include "pipe.h"
#include <vector>

struct game {
    const char* WINDOW_TITLE = "flappy bird";
    Audio audio;
    Graphic graphic;

    game(SDL_Texture* birdTexture);
    bool showMenu(bool& running,SDL_Renderer* renderer);
    bool running;
    bool isrunning() { return running; }
    void renderMenu(int selectedOption,SDL_Renderer* renderer);
    void handleEvent(bool& running, Mix_Chunk* flapSound,SDL_Renderer* renderer,int &score);
    void spawnpipe(SDL_Texture* pipeTexture);
    void renderScore(SDL_Renderer* renderer, int score);
    void update();
    bool checkcollision(const bird& b, const pipe& p) const;
    bool checkGameOver(SDL_Renderer* renderer, SDL_Texture* gameover,SDL_Texture *background,int score, Mix_Chunk* hitSound, bool& running);
    void restartGame(SDL_Texture* birdTexture);
    void cleanup(SDL_Texture* background, SDL_Texture* birdTexture, SDL_Texture* pipeTexture,
                 SDL_Texture* gameover, Mix_Chunk* flapSound, Mix_Chunk* hitSound,
                 Mix_Music* backgroundMusic);

    bird flappy;
    std::vector<pipe> pipes;
};

#endif // GAME_H
