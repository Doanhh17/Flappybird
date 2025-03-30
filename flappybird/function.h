#pragma once
#include"lib.h"
void init();
void resetGame();
bool checkCollision();
void update();
void RENDER();
SDL_Texture* loadTexture(const char* file);
void renderText(const string& text, int x, int y, SDL_Color color, TTF_Font* usedFont);
void drawButton(const SDL_Rect& rect, const string& text, bool hovered);
void quit();