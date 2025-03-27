#include "audio.h"
#include <SDL.h>
#include <iostream>

Audio::Audio() : flapSound(nullptr), hitSound(nullptr), backgroundMusic(nullptr) {}

Audio::~Audio() {
    Mix_FreeChunk(flapSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
}

bool Audio::loadSounds() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! Error: %s", Mix_GetError());
        return false;
    }

    flapSound = Mix_LoadWAV("flapSound.wav");
    hitSound = Mix_LoadWAV("hitSound.wav");
    backgroundMusic = Mix_LoadMUS("backgroundmusic.mp3");

    return flapSound && hitSound && backgroundMusic;
}

void Audio::playSound(Mix_Chunk* sound) {
    if (sound) {
        if (Mix_PlayChannel(-1, sound, 0) == -1) {
            SDL_Log("Error playing sound: %s", Mix_GetError());
        }
    } else {
        SDL_Log("Sound is NULL, cannot play!");
    }
}

Mix_Chunk* Audio::getFlapSound() const { return flapSound; }
Mix_Chunk* Audio::getHitSound() const { return hitSound; }
Mix_Music* Audio::getBackgroundMusic() const { return backgroundMusic; }
