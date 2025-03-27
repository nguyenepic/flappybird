#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>

struct Audio {
    Mix_Chunk* flapSound;
    Mix_Chunk* hitSound;
    Mix_Music* backgroundMusic;

    Audio();
    ~Audio();
    bool loadSounds();
    void playSound(Mix_Chunk* sound);
    Mix_Chunk* getFlapSound() const;
    Mix_Chunk* getHitSound() const;
    Mix_Music* getBackgroundMusic() const;
};
#endif // AUDIO_H
