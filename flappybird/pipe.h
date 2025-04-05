#pragma once

#include <SDL.h>

struct uppercol {
    int x, heigh;
    bool passed;
    int speedYcol;
    uppercol(int a, int b) {
        x = a;
        heigh = b;
        passed = false;
        speedYcol = rand() % 4 ;
        int list[2] = { speedYcol, -speedYcol };
        speedYcol = list[ rand() % 2];
    }
};