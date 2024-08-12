#include "input.h"
#include <raylib.h>

int handleInput(int *row, int *col, char ch, mode *md, int *num, Scheme *scheme) {
  switch (ch) {
  case 'h': {
    if (*col > 0) {
      (*col)--;
    }
    return -1;
    break;
  }
  case 'l': {
    if (*col < 8) {
      (*col)++;
    }
    return -1;
    break;
  }
  case 'j': {
    if (*row < 8) {
      (*row)++;
    }
    return -1;
    break;
  }
  case 'k': {
    if (*row > 0) {
      (*row)--;
    }
    return -1;
    break;
  }
  case 'm': {
    (*md) = MISSING;
    return -1;
    break;
  }
  case 'n': {
    (*md) = NOTE;
    return -1;
    break;
  }
  case 'i': {
    (*md) = INSERT;
    return -1;
    break;
  }
  case 'v': {
    (*md) = VISUAL;
    return -2;
    break;
  }
  case 'r': {
    return -3;
    break;
  }
  case 'c': {
    return -4;
    break;
  }
  default:
    if (ch > '0' && ch <= '9') {
      (*num) = ch - '0';
      return 0;
    }
    if (IsKeyPressed(KEY_TAB)) {
      scheme->th = scheme->th == 1 ? 0 : scheme->th + 1;
      updateColors(scheme);
    }
  }
  return 1;
}
