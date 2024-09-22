#include "Music.h"

// Initialize SDL and SDL_mixer for audio playback
// Returns true if initialization is successful, false otherwise
bool initSDL() {
    // Initialize SDL with audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer with support for MP3 and OGG formats
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "Mix_Init Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Open the audio device with a frequency of 44100Hz, default format, stereo sound, and a 2048 byte chunk size
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true; // SDL and SDL_mixer initialized successfully
}

// Global pointer to the music being played
Mix_Music* music = nullptr;

// Load a music file from the specified path and play it
// If the music is muted, the volume is set to 0
void loadAndPlaySong(const std::string& songPath) {
    // Load the music file from the given path
    music = Mix_LoadMUS(songPath.c_str());
    if (!music) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        return;
    }

    // Play the loaded music on loop (-1 means loop indefinitely)
    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music); // Free the music if it fails to play
        return;
    }

    // Set volume to 0 if the music should start muted
    if (isMuted) {
        Mix_VolumeMusic(0);
    }

    std::cout << "Playing song: " << songPath << std::endl;
}

// Clean up and close SDL and SDL_mixer resources
void cleanUpSDL() {
    if (music) {
        Mix_FreeMusic(music); // Free the music resource
    }
    Mix_CloseAudio(); // Close the audio device
    Mix_Quit(); // Quit SDL_mixer
    SDL_Quit(); // Quit SDL
}

// Start playing music from the specified file
// If the file does not exist in the provided path, it appends the default music directory
void startMusic(std::string inputfile) {
    if (!FileExists(inputfile)) {
        // Append default music directory if the file is not found
        inputfile = MUSIC_DIR + "/" + inputfile;
    }

    loadAndPlaySong(inputfile); // Load and play the song
}
