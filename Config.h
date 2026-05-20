#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <SDL3/SDL.h>

/**
 * @brief Central place for important constants and helper functions.
 * Custom SDL event types, stored in the UserEvents namespace.
 * General configuration options, stored in the Config namespace. This includes settings like window dimensions and color schemes.
 * Configuration options for the level editor are stored in the Config::Editor namespace.
 */
namespace UserEvents{
#ifdef WITH_EDITOR
    inline Uint32 ACTOR_DRAG{SDL_RegisterEvents(1)};
    inline Uint32 LOAD_LEVEL{SDL_RegisterEvents(1)};
    inline Uint32 SAVE_LEVEL{SDL_RegisterEvents(1)};
    inline Uint32 SAVE_AND_PLAY_LEVEL{SDL_RegisterEvents(1)};
    inline Uint32 LEVEL_EDIT{SDL_RegisterEvents(1)};
#endif
    inline Uint32 LAUNCH_LEVEL{SDL_RegisterEvents(1)};
}

namespace Config {
    inline const std::vector BUTTON_COLORS{
        SDL_Color{15, 15, 15, 255},  // Normal
        SDL_Color{15, 155, 15, 255}, // Hover
        SDL_Color{225, 15, 15, 255}, // Active
        SDL_Color{60, 60, 60, 255}   // Disabled
    };

    inline constexpr SDL_Color FONT_COLOR{
        255, 255, 255, 255
      };

    inline const std::string BASE_PATH{
        SDL_GetBasePath()
      };

    inline const std::string FONT{
        BASE_PATH + "Assets/Rubik-SemiBold.ttf"
      };

}

#ifdef WITH_EDITOR
namespace Config::Editor {
    inline constexpr Uint8 Version = 1;

    // Configs for an editor's level
    inline constexpr int PADDING = 10;
    inline constexpr int HORIZONTAL_GRID_SNAP = 50;
    inline constexpr int VERTICAL_GRID_SNAP = 25;
    inline constexpr Uint8 GRID_WIDTH = 13;
    inline constexpr Uint8 GRID_HEIGHT = 6;
    inline constexpr int LEVEL_WIDTH = HORIZONTAL_GRID_SNAP * GRID_WIDTH;
    inline constexpr int LEVEL_HEIGHT = VERTICAL_GRID_SNAP * GRID_HEIGHT;
    inline constexpr SDL_Color LEVEL_BACKGROUND = {50, 50, 50, 255};

    // Configs for the editor's actor menu
    inline constexpr int ACTOR_MENU_WIDTH = 70;
    inline constexpr int ACTOR_MENU_POSITION_X = LEVEL_WIDTH;
    inline constexpr SDL_Color ACTOR_MENU_BACKGROUND = {15, 15, 15, 255};

    // Configs for the Editor's window
    inline constexpr std::string WINDOW_TITLE = "Editor";
    inline constexpr int WINDOW_WIDTH = LEVEL_WIDTH + ACTOR_MENU_WIDTH;
    inline constexpr int WINDOW_HEIGHT = LEVEL_HEIGHT + 50;
    inline constexpr SDL_Color WINDOW_BACKGROUND = {35, 35, 35, 255};

}
#endif

namespace Config::Engine {
    inline constexpr std::string WINDOW_TITLE = "RPG";
    inline constexpr int WINDOW_WIDTH = 720;
    inline constexpr int WINDOW_HEIGHT = 400;
    inline constexpr SDL_Color WINDOW_BACKGROUND = {35, 35, 35, 255};
}

inline void CheckSDLError(const std::string& Msg) {
#ifdef CHECK_ERRORS
    const char* error = SDL_GetError();
    if (*error != '\0') {
        std::cerr << Msg << " Error: " << error << std::endl;
        SDL_ClearError();
    }
#endif
}