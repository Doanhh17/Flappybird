#include"lib.h"
#include"constant.h"
#include"global.h"
SDL_Texture* loadTexture(const char* file) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        cout << "Error loading image: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderText(const string& text, int x, int y, SDL_Color color, TTF_Font* usedFont) {
    SDL_Surface* surface = TTF_RenderText_Blended(usedFont, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(render, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(const SDL_Rect& rect, const string& text, bool hovered) {
    // Nền nút
    SDL_SetRenderDrawColor(render, hovered ? 100 : 50, hovered ? 100 : 50, hovered ? 100 : 50, 200);
    SDL_RenderFillRect(render, &rect);

    // Viền nút
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &rect);

    // Text căn giữa
    SDL_Color textColor = hovered ? SDL_Color{ 255, 255, 0 } : SDL_Color{ 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Rect textRect = {
        rect.x + (rect.w - textSurface->w) / 2,
        rect.y + (rect.h - textSurface->h) / 2,
        textSurface->w,
        textSurface->h
    };
    SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, textSurface), nullptr, &textRect);
    SDL_FreeSurface(textSurface);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthwindow, heighwindow, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Font tiêu đề lớn hơn
    font = TTF_OpenFont("font.ttf", 24);
    titleFont = TTF_OpenFont("font.ttf", 48);
    if (!font || !titleFont) {
        cout << "Error loading font: " << TTF_GetError() << endl;
        run = false;
        return;
    }

    birdTexture1 = loadTexture("bird1.png");
    birdTexture2 = loadTexture("bird2.png");
    birdTexture3 = loadTexture("bird3.png");
    pipeTexture = loadTexture("pipe.png");
    backgroundTexture = loadTexture("background.png");

    jumpSound = Mix_LoadWAV("jump.wav");
    hitSound = Mix_LoadWAV("hit.wav");
    scoreSound = Mix_LoadWAV("score.wav");
    backgroundMusic = Mix_LoadMUS("background.mp3");

    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1);
    }
}

void resetGame() {
    speedcol = 5;
    flappybird.y = heighwindow / 3;
    birdV = 0;
    col_list.clear();
    col_list.push_back(uppercol(widthwindow, rand() % (heighwindow - dis_tance2cols - 50)));
    score = 0;
    run = true;
    isPaused = false;
    gameOver = false; // Reset trạng thái game over
}

bool checkCollision() {
    for (auto& col : col_list) {
        if (flappybird.y < 0 || flappybird.y + birdsize > heighwindow) return true;
        if (flappybird.x + birdsize > col.x && flappybird.x < col.x + widthcol) {
            if (flappybird.y < col.heigh || flappybird.y + birdsize > col.heigh + dis_tance2cols) {
                return true;
            }
        }
    }
    return false;
}
bool checkcollisionbutton(SDL_Rect a,int buttonx,int buttony) {
    return (buttonx >= a.x && buttonx <= a.x + a.w &&
        buttony >= a.y && buttony <= a.y + a.h);
}
void update() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            run = false;
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX = e.motion.x;
            mouseY = e.motion.y;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if (inMenu) {
                if (checkcollisionbutton(startRect, x, y)) {
                    inMenu = false;
                    resetGame();
                }
                if (checkcollisionbutton(quitRect, x, y)) {
                    run = false;
                }
                if (checkcollisionbutton(helpRect, x, y)) {
                    inhelp = true;
                    inMenu = false;
                }
            }
            else if(inhelp){
                if (checkcollisionbutton(backRect, x, y)) {
                    inhelp = false;
                    inMenu = true;
                }
            }
            else if (isPaused) {
                if (checkcollisionbutton(resumeRect, x, y)) {
                    isPaused = false;
                }
                if (checkcollisionbutton(restartRect, x, y)) {
                    resetGame();
                }
                if (checkcollisionbutton(pauseQuitRect, x, y)) {
                    inMenu = true;
                    isPaused = false;
                }
            }
            else if (gameOver) { // Thêm xử lý sự kiện cho game over
                if (checkcollisionbutton(gameOverRestartRect, x, y)) {
                    resetGame();
                }
                if (checkcollisionbutton(gameOverMenuRect, x, y)) {
                    inMenu = true;
                    gameOver = false;
                }
            }
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_p && !inMenu && !gameOver && !inhelp) { // Không cho pause khi game over
                isPaused = !isPaused;
            }
            else if (e.key.keysym.sym == SDLK_SPACE && !inMenu && !isPaused && !gameOver && !inhelp) {
                birdV = jump;
                Mix_PlayChannel(-1, jumpSound, 0);
            }
        }
    }
    if (!inMenu && !isPaused && !gameOver&&!inhelp) { // Thêm điều kiện !gameOver
            animationDelay++;
            if (animationDelay > ANIMATION_SPEED) {
                animationFrame = (animationFrame + 1) % 3;
                animationDelay = 0;
            }

            birdV += g;
            flappybird.y += birdV;

            for (auto& col : col_list) {
                col.x -= speedcol;
            }

            if (!col_list.empty() && col_list[0].x + widthcol < -2) {
                col_list.erase(col_list.begin());
            }

            if (col_list.empty() || col_list.back().x + widthcol < widthwindow / 2) {
                col_list.push_back(uppercol(widthwindow, rand() % (heighwindow - dis_tance2cols - 50)));
            }

            for (auto& col : col_list) {
                if (!col.passed && flappybird.x > col.x + widthcol) {
                    score++;
                    if (score % 5 == 0) speedcol += 1;
                    col.passed = true;
                    Mix_PlayChannel(-1, scoreSound, 0);
                }
            }

            if (checkCollision()) {
                Mix_PlayChannel(-1, hitSound, 0);
                if (score > highScore) highScore = score;
                gameOver = true; // Thay đổi trạng thái thành game over thay vì về menu
            }
    }
}
void RENDER() {
    SDL_RenderClear(render);
    SDL_RenderCopy(render, backgroundTexture, nullptr, nullptr);
    if (inMenu) {
        // Tiêu đề game - căn giữa
        SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "FLAPPY BIRD", { 255, 215, 0 });
        SDL_Rect titleRect = { widthwindow / 2 - titleSurface->w / 2, TITLE_HEIGH, titleSurface->w, titleSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, titleSurface), nullptr, &titleRect);
        SDL_FreeSurface(titleSurface);

        // Các nút bấm
        bool hoverStart = checkcollisionbutton(startRect, mouseX, mouseY);
        bool hoverQuit = checkcollisionbutton(quitRect, mouseX, mouseY);
        bool hoverhelp = checkcollisionbutton(helpRect, mouseX, mouseY);
        drawButton(startRect, "START GAME", hoverStart);
        drawButton(helpRect, "HELP", hoverhelp);
        drawButton(quitRect, "QUIT", hoverQuit);

        // High score - căn giữa dưới cùng
        string highScoreText = "HIGH SCORE: " + to_string(highScore);
        SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, highScoreText.c_str(), { 255, 255, 255 });
        SDL_Rect scoreRect = { widthwindow / 2 - scoreSurface->w / 2, SCORE_BOTTOM, scoreSurface->w, scoreSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, scoreSurface), nullptr, &scoreRect);
        SDL_FreeSurface(scoreSurface);
    }
    else if (inhelp) {
        bool hoverback = checkcollisionbutton(backRect,mouseX,mouseY);
        drawButton(backRect, "BACK TO MENU", hoverback);
        renderText("Press SPACE to control the bird to fly through pipes", startRect.x-200, startRect.y-100, { 255,255,0 }, font);
        renderText("Each passed pipe: +1 point. Difficulty increases with score", startRect.x - 230, startRect.y - 100+BUTTON_HEIGHT, { 255,255,0 }, font);
    }
    else if (isPaused) {
        // Tiêu đề pause - căn giữa
        SDL_Surface* pauseSurface = TTF_RenderText_Blended(titleFont, "GAME PAUSED", { 255, 255, 255 });
        SDL_Rect pauseRect = { widthwindow / 2 - pauseSurface->w / 2, TITLE_HEIGH, pauseSurface->w, pauseSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, pauseSurface), nullptr, &pauseRect);
        SDL_FreeSurface(pauseSurface);

        // Điểm số - căn giữa
        string scoreText = "SCORE: " + to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, scoreText.c_str(), { 255, 255, 255 });
        SDL_Rect scoreRect = { widthwindow / 2 - scoreSurface->w / 2, TITLE_HEIGH + 100, scoreSurface->w, scoreSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, scoreSurface), nullptr, &scoreRect);
        SDL_FreeSurface(scoreSurface);

        // Các nút bấm
        bool hoverResume = (mouseX >= resumeRect.x && mouseX <= resumeRect.x + resumeRect.w &&
            mouseY >= resumeRect.y && mouseY <= resumeRect.y + resumeRect.h);
        bool hoverRestart = (mouseX >= restartRect.x && mouseX <= restartRect.x + restartRect.w &&
            mouseY >= restartRect.y && mouseY <= restartRect.y + restartRect.h);
        bool hoverQuit = (mouseX >= pauseQuitRect.x && mouseX <= pauseQuitRect.x + pauseQuitRect.w &&
            mouseY >= pauseQuitRect.y && mouseY <= pauseQuitRect.y + pauseQuitRect.h);

        drawButton(resumeRect, "RESUME", hoverResume);
        drawButton(restartRect, "RESTART", hoverRestart);
        drawButton(pauseQuitRect, "QUIT TO MENU", hoverQuit);
    }
    else if (gameOver) { // Thêm phần render game over
        // Vẽ game bình thường ở background
        SDL_Texture* currentBirdTexture = nullptr;
        switch (animationFrame) {
        case 0: currentBirdTexture = birdTexture1; break;
        case 1: currentBirdTexture = birdTexture2; break;
        case 2: currentBirdTexture = birdTexture3; break;
        default: currentBirdTexture = birdTexture1; break;
        }
        SDL_RenderCopy(render, currentBirdTexture, nullptr, &flappybird);

        for (auto& col : col_list) {
            SDL_Rect upper = { col.x, 0, widthcol, col.heigh };
            SDL_Rect under = { col.x, col.heigh + dis_tance2cols, widthcol, heighwindow - col.heigh - dis_tance2cols };
            SDL_RenderCopy(render, pipeTexture, nullptr, &upper);
            SDL_RenderCopyEx(render, pipeTexture, nullptr, &under, 0, nullptr, SDL_FLIP_VERTICAL);
        }

        // Vẽ overlay mờ
        SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 200);
        SDL_Rect overlay = { 0, 0, widthwindow, heighwindow };
        SDL_RenderFillRect(render, &overlay);
        SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_NONE);

        // Tiêu đề Game Over
        SDL_Surface* gameOverSurface = TTF_RenderText_Blended(titleFont, "GAME OVER", { 255, 50, 50 });
        SDL_Rect gameOverRect = { widthwindow / 2 - gameOverSurface->w / 2, TITLE_HEIGH, gameOverSurface->w, gameOverSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, gameOverSurface), nullptr, &gameOverRect);
        SDL_FreeSurface(gameOverSurface);

        // Hiển thị điểm
        string scoreText = "YOUR SCORE: " + to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, scoreText.c_str(), { 255, 255, 255 });
        SDL_Rect scoreRect = { widthwindow / 2 - scoreSurface->w / 2, heighwindow / 2 - 50, scoreSurface->w, scoreSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, scoreSurface), nullptr, &scoreRect);
        SDL_FreeSurface(scoreSurface);

        // Hiển thị high score
        string highScoreText = "HIGH SCORE: " + to_string(highScore);
        SDL_Surface* highScoreSurface = TTF_RenderText_Blended(font, highScoreText.c_str(), { 255, 255, 255 });
        SDL_Rect highScoreRect = { widthwindow / 2 - highScoreSurface->w / 2, heighwindow / 2, highScoreSurface->w, highScoreSurface->h };
        SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, highScoreSurface), nullptr, &highScoreRect);
        SDL_FreeSurface(highScoreSurface);

        // Các nút bấm
        bool hoverRestart = (mouseX >= gameOverRestartRect.x && mouseX <= gameOverRestartRect.x + gameOverRestartRect.w &&
            mouseY >= gameOverRestartRect.y && mouseY <= gameOverRestartRect.y + gameOverRestartRect.h);
        bool hoverMenu = (mouseX >= gameOverMenuRect.x && mouseX <= gameOverMenuRect.x + gameOverMenuRect.w &&
            mouseY >= gameOverMenuRect.y && mouseY <= gameOverMenuRect.y + gameOverMenuRect.h);

        drawButton(gameOverRestartRect, "RESTART", hoverRestart);
        drawButton(gameOverMenuRect, "QUIT TO MENU", hoverMenu);
    }
    else {
        // Game play
        SDL_Texture* currentBirdTexture = nullptr;
        switch (animationFrame) {
        case 0: currentBirdTexture = birdTexture1; break;
        case 1: currentBirdTexture = birdTexture2; break;
        case 2: currentBirdTexture = birdTexture3; break;
        default: currentBirdTexture = birdTexture1; break;
        }
        SDL_RenderCopy(render, currentBirdTexture, nullptr, &flappybird);

        for (auto& col : col_list) {
            SDL_Rect upper = { col.x, 0, widthcol, col.heigh };
            SDL_Rect under = { col.x, col.heigh + dis_tance2cols, widthcol, heighwindow - col.heigh - dis_tance2cols };
            SDL_RenderCopy(render, pipeTexture, nullptr, &upper);
            SDL_RenderCopyEx(render, pipeTexture, nullptr, &under, 0, nullptr, SDL_FLIP_VERTICAL);
        }

        // Hiển thị điểm và hướng dẫn
        renderText("Score: " + to_string(score), 20, 20, { 255, 255, 255 }, font);
        renderText("Press P to pause", 20, heighwindow - 40, { 255, 255, 255 }, font);
    }

    SDL_RenderPresent(render);
}

void quit() {
    Mix_HaltMusic();
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(scoreSound);
    Mix_CloseAudio();

    SDL_DestroyTexture(birdTexture1);
    SDL_DestroyTexture(birdTexture2);
    SDL_DestroyTexture(birdTexture3);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);

    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}
