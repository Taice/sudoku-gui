#include "color.h"

void updateColors(Scheme *scheme) {
  switch (scheme->th) {
  case NORMAL:
    scheme->gaps = BLACK;
    break;
  case GRUVBOX:
    scheme->gaps = (Color){60, 56, 54, 255};
    break;
  }
  switch (scheme->th) {
  case NORMAL:
    scheme->background = WHITE;
    break;
  case GRUVBOX:
    scheme->background = (Color){251, 241, 199, 255};
    break;
  }
  switch (scheme->th) {
  case NORMAL:
    scheme->text = BLACK;
    break;
  case GRUVBOX:
    scheme->text = (Color){40, 40, 40, 255};
    break;
  }
  switch (scheme->th) {
  case NORMAL:
    scheme->notes = DARKGRAY;
    break;
  case GRUVBOX:
    scheme->notes = (Color){80, 73, 69, 255};
    break;
  }
  switch (scheme->th) {
  case NORMAL:
    scheme->highlight = (Color){69, 133, 136, 255};
    break;
  case GRUVBOX:
    scheme->highlight = (Color){250, 189, 100, 255};
    break;
  }
  switch (scheme->th) {
  case NORMAL:
    scheme->lowlight = (Color){131, 165, 152, 255};
    break;
  case GRUVBOX:
    scheme->lowlight = (Color){213, 196, 161, 255};
    break;
  }
}
