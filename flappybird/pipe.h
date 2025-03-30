#pragma once

#include <SDL.h>

struct uppercol {
    int x, heigh;
    bool passed;

    uppercol(int a, int b) {
        x = a;
        heigh = b;
        passed = false;
    }
};