#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <exception>

class UI {
private:
    struct Button {
        int x;
        int y;
        int w;
        int h;

        std::string text;
        Uint32 color;
        Uint32 textColor;

        std::function<void()> onClick;
    };

    int W;
    int H;

    TTF_Font* font;
    Uint32* pixels;

    std::vector<Button> buttons;

public:
    UI(int w, int h, TTF_Font* font_) : W(w), H(h), font(font_) {
        try {
            pixels = new Uint32[W * H];
            clear();
        } catch (const std::exception& e) {
            std::cerr << "Exception in UI constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in UI constructor"
                      << std::endl;
            throw;
        }
    }

    ~UI() {
        delete[] pixels;
    }

    void clear() noexcept {
        for (int i = 0; i < W * H; i++)
            pixels[i] = 0x00000000;
    }

    void addButton(
        int x,
        int y,
        int w,
        int h,
        const std::string& text,
        std::function<void()> onClick,
        Uint32 color = 0x888888FF,
        Uint32 textColor = 0xFFFFFFFF
    ) {
        try {
            buttons.push_back({
                x, y, w, h,
                text,
                color,
                textColor,
                onClick
            });
        } catch (const std::exception& e) {
            std::cerr << "Exception in UI::addButton: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in UI::addButton"
                      << std::endl;
            throw;
        }
    }

    void build() {
        try {
            clear();

            for (const Button& btn : buttons) {
                for (int py = btn.y; py < btn.y + btn.h; py++) {
                    for (int px = btn.x; px < btn.x + btn.w; px++) {
                        if (px < 0 || px >= W || py < 0 || py >= H)
                            continue;

                        pixels[py * W + px] = btn.color;
                    }
                }

                if (!font || btn.text.empty())
                    continue;

                SDL_Color c {
                    static_cast<Uint8>((btn.textColor >> 16) & 0xFF),
                    static_cast<Uint8>((btn.textColor >> 8) & 0xFF),
                    static_cast<Uint8>(btn.textColor & 0xFF),
                    static_cast<Uint8>((btn.textColor >> 24) & 0xFF)
                };

                SDL_Surface* textSurface = TTF_RenderUTF8_Blended(
                    font, btn.text.c_str(), c
                );
                if (!textSurface)
                    continue;

                SDL_Surface* converted = SDL_ConvertSurfaceFormat(
                    textSurface,
                    SDL_PIXELFORMAT_RGBA8888,
                    0
                );
                SDL_FreeSurface(textSurface);

                if (!converted)
                    continue;

                int tx = btn.x + (btn.w - converted->w) / 2;
                int ty = btn.y + (btn.h - converted->h) / 2;

                Uint32* src = static_cast<Uint32*>(converted->pixels);
                int pitchPixels = converted->pitch / 4;

                for (int y = 0; y < converted->h; y++) {
                    for (int x = 0; x < converted->w; x++) {
                        int dstX = tx + x;
                        int dstY = ty + y;

                        if (dstX < 0 || dstX >= W ||
                            dstY < 0 || dstY >= H)
                            continue;

                        Uint32 px = src[y * pitchPixels + x];

                        Uint8 r, g, b, a;
                        SDL_GetRGBA(
                            px, converted->format,
                            &r, &g, &b, &a
                        );

                        if (a == 0) continue;

                        pixels[dstY * W + dstX] = SDL_MapRGBA(
                            SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888),
                            r, g, b, a
                        );
                    }
                }

                SDL_FreeSurface(converted);
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in UI::build: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in UI::build"
                      << std::endl;
            throw;
        }
    }

    void hitTest(int mx, int my) {
        for (const Button& btn : buttons) {
            if (mx >= btn.x && mx < btn.x + btn.w &&
                my >= btn.y && my < btn.y + btn.h) {
                if (btn.onClick) {
                    try {
                        btn.onClick();
                    } catch (const std::exception& e) {
                        std::cerr << "Exception in UI button callback: "
                                  << e.what()
                                  << std::endl;
                    } catch (...) {
                        std::cerr << "Unknown exception in UI button callback"
                                  << std::endl;
                    }
                }
                return;
            }
        }
    }

    void clearButtons() noexcept {
        buttons.clear();
    }

    Uint32* getPixels() const noexcept {
        return pixels;
    }
};