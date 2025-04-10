﻿#pragma once
#include"lib.h"
#include"pipe.h"
extern SDL_Window* window;
extern SDL_Renderer* render;
extern SDL_Texture* birdTexture1;
extern SDL_Texture* birdTexture2;
extern SDL_Texture* birdTexture3;
extern SDL_Texture* pipeTexture1;
extern SDL_Texture* pipeTexture2;
extern SDL_Texture* backgroundTexture;
extern TTF_Font* font;
extern TTF_Font* titleFont;
extern Mix_Music* backgroundMusic;
extern Mix_Chunk* jumpSound;
extern Mix_Chunk* hitSound;
extern Mix_Chunk* scoreSound;
extern SDL_Rect flappybird;
extern vector<uppercol> col_list;
extern bool run;
extern bool inMenu;
extern bool inhelp;
extern bool isPaused;
extern bool gameOver;
extern int speedcol;
extern int score;
extern int birdV;
extern int animationFrame;
extern int animationDelay;
extern int mouseX;
extern int mouseY;

extern SDL_Rect startRect;
extern SDL_Rect helpRect;
extern SDL_Rect quitRect;
extern SDL_Rect resumeRect;
extern SDL_Rect restartRect;
extern SDL_Rect pauseQuitRect;
extern SDL_Rect gameOverRestartRect;
extern SDL_Rect gameOverMenuRect;
extern SDL_Rect backRect;
