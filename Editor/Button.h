#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "Editor/Text.h"

/**
 * @enum ButtonState 
 * @brief Enum class that lists the state a button can be in. 
 */
enum class ButtonState {
    Normal = 0,
    Hover = 1,
    Active = 2,
    Disabled = 3
};

namespace Editor {
    /// Scene class is forward declarated to prevent circular dependency.
    class Scene;
    
    /**
     * @class Button
     * @brief An interactive button for the editor UI.
     * 
     * Owns a Text label and an SDL_Rect hit-box. On each frame it responds
     * to SDL mouse events, updates its ButtonState, and draws itself with
     * the corresponding color from Config::BUTTON_COLORS.
     *
     * Subclasses override HandleLeftClick() to implement specific actions.
     */
    class Button {
    public:
        Button(Scene& ParentScene, const std::string& Text, SDL_Rect Rect);

        virtual void HandleLeftClick() {}
        void HandleEvent(const SDL_Event& E);
        void Render(SDL_Surface* Surface);
        void Tick(float DeltaTime) {}

        ButtonState GetState() const;

        void SetState(ButtonState NewState);

    private:
        /// The Scene that owns this button.
        Scene& ParentScene;

        /// Current interactive state of the button.
        ButtonState State{ ButtonState::Normal };

        /// Text label rendered inside the button rectangle.
        Text ButtonText;

        /// Bounding rectangle used for rendering and hit-testing.
        SDL_Rect Rect;
    };
}