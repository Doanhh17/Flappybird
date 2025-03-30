#include"lib.h"
#include"global.h"
#include"constant.h"
#include"function.h"
int main(int argc, char* argv[]) {
    init();
    while (run) {
        update();
        RENDER();
        SDL_Delay(16);
    }
    quit();
    return 0;
}