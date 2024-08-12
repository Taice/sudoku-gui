#pragma once
#include <raylib.h>

typedef enum {
  NORMAL=0,
  GRUVBOX=1,
} Theme;

typedef struct {
  Theme th;
  Color gaps, background, text, notes, highlight, lowlight;
} Scheme;

void updateColors(Scheme *scheme);
