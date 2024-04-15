
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "macros.h"
#include "types.h"
#include "easing.h"
#include "game.h"
#include "assets.h"
