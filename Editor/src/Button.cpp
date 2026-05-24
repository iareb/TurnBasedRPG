#include "Editor/Button.h"
#include "Editor/Scene.h"
#include "Config.h"

using namespace Editor;

/**
 * @brief Initialises the button with a text label, a bounding rectangle,
 *        and a reference to the owning scene.
 * 
 * The Text object is constructed with a fixed font size defined in Config.h.
 */
Button::Button(
	Scene& ParentScene, 
	const std::string& Text, 
	SDL_Rect Rect
) : ButtonText{ Text, Config::FONT_SIZE },
	Rect{ Rect },
	ParentScene{ ParentScene } 
{}

/**
 * @brief Returns the current state of the button.
 */
ButtonState Button::GetState() const {
	return State;
}

/**
 * @brief Sets a new state for the button.
 * @param NewState The new state of the button
 */
void Button::SetState(ButtonState NewState) {
	State = NewState;
}

/**
 * @brief Processes a SDL_Event and updates the button state.
 * 
 * Two event types are handled:
 *  - SDL_EVENT_MOUSE_BUTTON_DOWN: fires HandleLeftClick() when the left
 *    mouse button is released over a button in the Hover state.
 *  - SDL_EVENT_MOUSE_MOTION: transitions between Normal and Hover depending
 *    on whether the cursor is inside the hit-box. Motion events are skipped
 *    when the parent scene does not have mouse focus, avoiding ghost hovers
 *    when the window is in the background.
 * 
 * @param E The SDL Event to process
 */
void Button::HandleEvent(const SDL_Event& E) {
    using enum ButtonState;
    if (E.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        E.button.button == SDL_BUTTON_LEFT &&
        State == Hover
        ) {
        HandleLeftClick();
    }
    else if (
        E.type == SDL_EVENT_MOUSE_MOTION &&
        ParentScene.HasMouseFocus()
        ) {
        SDL_Point Pos{ (int)E.motion.x, (int)E.motion.y };
        bool Hovering(SDL_PointInRect(&Pos, &Rect));
        if (State == Normal && Hovering) {
            State = Hover;
        }
        else if (State == Hover && !Hovering) {
            State = Normal;
        }
    }
}

/**
 * @brief Draws the button background and its centered text label.
 * 
 * Looks up the fill color from Config::BUTTON_COLORS using the current
 * ButtonState cast to an index, maps it to the surface's pixel format,
 * fills the bounding rectangle, then delegates text rendering to ButtonText.
 *
 * @param Surface Destination surface to render onto.
 */
void Button::Render(SDL_Surface* Surface) {
    using namespace Config;
    auto [r, g, b, a] = BUTTON_COLORS[static_cast<int>(State)];

    const auto* Fmt{ SDL_GetPixelFormatDetails(
      Surface->format
    ) };

    SDL_FillSurfaceRect(
        Surface, &Rect, SDL_MapRGB(
            Fmt, nullptr, r, g, b
        )
    );

    ButtonText.Render(Surface, &Rect);
}