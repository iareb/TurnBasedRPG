#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include "Config.h"

namespace Editor {
    /**
     * @class Image
     * @brief RAII wrapper around an SDL_Surface loaded from an image file.
     *
     * Loads an image from disk via SDL3_image and exposes a scaled blit
     * operation for rendering onto any target surface. The underlying
     * SDL_Surface is owned exclusively by this object and freed on destruction.
     */
    class Image {
    public:
        /// Empty Image with no surface loaded.
        Image() = default;

        /**
         * @brief Loads an image from the given file path.
         *
         * Supports any format recognized by SDL3_image (PNG, JPG, BMP, etc.).
         * Checks for SDL errors after loading.
         *
         * @param Path Absolute or relative path to the image file
         */
        Image(const std::string& Path)
        : ImageSurface{IMG_Load(Path.c_str())
        } {
            CheckSDLError("Loading Image");
        }

        /**
         * @brief Blits the image onto a target surface, scaled to fit the given rectangle
         *
         * Uses SDL_SCALEMODE_LINEAR for smooth interpolation during scaling.
         * Does nothing if the image was default-constructed or has been moved from.
         *
         * @param Surface Destination surface to render onto
         * @param Rect Destination area; the image is scaled to fill it entirely
         */
        void Render(SDL_Surface* Surface, SDL_Rect Rect) const {
            SDL_BlitSurfaceScaled(
              ImageSurface, nullptr, Surface, &Rect,
              SDL_SCALEMODE_LINEAR
            );
        }

        /**
         * @brief Move constructor - transfers surface ownership from another Image.
         *
         * The moved-from object is left in a valid but empty state (null surface),
         * preventing a double-free on destruction.
         *
         * @param Other The image to move from; its surface pointer is nulled out
         */
        Image(Image&& Other) noexcept
        : ImageSurface(Other.ImageSurface) {
            Other.ImageSurface = nullptr;
        }

        /**
         * @brief Destroys the SDL surface if one has been loaded.
         *
         * Safe to call on a default-constructed or moved-from instance,
         * as both leave ImageSurface as nullptr.
         */
        ~Image() {
            if (ImageSurface) {
                SDL_DestroySurface(ImageSurface);
            }
        }

        /// Copying is disabled — SDL_Surface* must have a single owner.
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

    private:
        SDL_Surface* ImageSurface = nullptr;
    };
}