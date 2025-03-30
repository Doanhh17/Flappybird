#include"lib.h"
#include"constant.h"
#include"pipe.h"
vector<uppercol> col_list;
SDL_Window* window = nullptr;
SDL_Renderer* render = nullptr;
SDL_Texture* birdTexture1 = nullptr;
SDL_Texture* birdTexture2 = nullptr;
SDL_Texture* birdTexture3 = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
TTF_Font* font = nullptr;
TTF_Font* titleFont = nullptr;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* hitSound = nullptr;
Mix_Chunk* scoreSound = nullptr;

SDL_Rect flappybird = { widthwindow / 8, heighwindow-100, birdsize, birdsize };
bool run = true;
bool inMenu = true;
bool isPaused = false;
bool gameOver = false; // Thêm biến gameOver mới
int score = 0, highScore = 0;
int birdV = 0;

int animationFrame = 0;
int animationDelay = 0;
int mouseX = 0, mouseY = 0;
// Menu chính - căn giữa hoàn hảo
SDL_Rect startRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 - BUTTON_HEIGHT - BUTTON_DISTANCE / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
SDL_Rect quitRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 + BUTTON_DISTANCE / 2, BUTTON_WIDTH, BUTTON_HEIGHT };

// Menu tạm dừng - căn giữa hoàn hảo
SDL_Rect resumeRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 - BUTTON_HEIGHT * 3/2 - BUTTON_DISTANCE, BUTTON_WIDTH, BUTTON_HEIGHT };
SDL_Rect restartRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
SDL_Rect pauseQuitRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 + BUTTON_HEIGHT * 1/2 + BUTTON_DISTANCE, BUTTON_WIDTH, BUTTON_HEIGHT };

// Menu game over - thêm mới
SDL_Rect gameOverRestartRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
SDL_Rect gameOverMenuRect = { widthwindow / 2 - BUTTON_WIDTH / 2, heighwindow / 2 + BUTTON_HEIGHT + BUTTON_DISTANCE, BUTTON_WIDTH, BUTTON_HEIGHT };

