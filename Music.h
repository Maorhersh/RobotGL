#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <fstream>
#include <iostream>
#include "ObjectGl.h"

// The default directory for music files
const std::string MUSIC_DIR = "music";

// A flag to indicate whether the music is initially muted
static bool isMuted = true;

/**
 * @brief Initializes SDL and SDL_mixer libraries.
 *
 * This function sets up SDL for audio processing and initializes SDL_mixer
 * to support various audio formats like MP3 and OGG.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool initSDL();

/**
 * @brief Loads a music file and plays it.
 *
 * This function takes the path to a music file, loads it, and plays it.
 * If the global `isMuted` flag is true, the music will start muted.
 *
 * @param songPath The path to the music file.
 */
void loadAndPlaySong(const std::string& songPath);

/**
 * @brief Cleans up SDL and SDL_mixer resources.
 *
 * This function frees the music resource, closes the audio device,
 * and shuts down SDL and SDL_mixer libraries.
 */
void cleanUpSDL();

/**
 * @brief Starts playing music from a specified file.
 *
 * This function checks if the specified music file exists. If not, it appends
 * the default music directory to the file path and then loads and plays the music.
 *
 * @param inputfile The name or path of the music file.
 */
void startMusic(std::string inputfile);
