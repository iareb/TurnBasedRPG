#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Config.h"

namespace Editor {
	/**
	 * @class Text
	 * @brief RAII wrapper for rendering text onto an SDL_Surface.
	 *
	 * Loads a TTF font at a given size, renders a text string into an owned
	 * SDL_Surface via SDL3_ttf, and blits it centered inside a target rectangle.
	 * The surface is re-generated automatically whenever the text content changes.
	 */
	class Text {
	public:
		/**
		 * @brief Loads the font and renders the initial text string.
		 *
		 * Opens Config::FONT at the requested size, then calls SetText()
		 * to produce the first TextSurface. Checks for SDL errors after
		 * each SDL/TTF call.
		 *
		 * @param InitialText The string to display on construction.
		 * @param FontSize    Point size passed to TTF_OpenFont.
		 */
		Text(const std::string& InitialText, int FontSize)
		: Content{ InitialText } 
		{
			Font = TTF_OpenFont(Config::FONT.c_str(), FontSize);
			CheckSDLError("Opening Font");
			SetText(InitialText);
		}

		/**
		 * @brief Frees the text surface and the font handle.
		 *
		 * Skips cleanup entirely if the SDL video subsystem has already been
		 * shut down, guarding against destruction-order issues at program exit.
		 */
		~Text() {
			if (!SDL_WasInit(SDL_INIT_VIDEO)) {
				return;
			}

			if (TextSurface) {
				SDL_DestroySurface(TextSurface);
			}

			if (Font) {
				TTF_CloseFont(Font);
			}
		}

		/// Copying is disabled - Font and TextSurface must have a single owner.
		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		/**
		 * @brief Replaces the displayed string and regenerates the text surface.
		 *
		 * Destroys the previous TextSurface before creating a new one with
		 * TTF_RenderText_Blended, using Config::FONT_COLOR as the foreground color.
		 * Checks for SDL errors after surface creation.
		 *
		 * @param NewText The new string to render.
		 */
		void SetText(const std::string& NewText) {
			Content = NewText;

			if (TextSurface) {
				SDL_DestroySurface(TextSurface);
			}
			 
			TextSurface = TTF_RenderText_Blended(
				Font, Content.c_str(), 0, Config::FONT_COLOR
			);

			CheckSDLError("Creating Text Surface");
		}

		/**
		 * @brief Blits the text surface centered inside the given rectangle.
		 *
		 * Computes a destination rect that centers the text both horizontally
		 * and vertically within @p Rect, then blits without scaling.
		 * Does nothing if TextSurface is null (e.g. after a failed SetText call).
		 *
		 * @param Surface  Destination surface to render onto.
		 * @param Rect     Bounding rectangle used as the centering reference;
		 *                 the text is never clipped or scaled to fit it.
		 */
		void Render(SDL_Surface* Surface, SDL_Rect* Rect) {
			if (!TextSurface) {
				return;
			}

			int TextW = TextSurface->w;
			int TextH = TextSurface->h;

			SDL_Rect Destination = {
				Rect->x + (Rect->w - TextW) / 2,
				Rect->y + (Rect->h - TextH) / 2,
				TextW, TextH
			};

			SDL_BlitSurface(
				TextSurface, nullptr,
				Surface, &Destination
			);
		}

	private:
		/// The current text string, kept in sync with TextSurface.
		std::string Content;

		/// TTF font handle loaded from Config::FONT.
		TTF_Font* Font = nullptr;

		/// Rendered surface for the current Content string; null if rendering failed.
		SDL_Surface* TextSurface = nullptr;
	};
}