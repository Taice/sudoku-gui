#include "color.h"
#include "framerate.h"
#include "input.h"
#include "mode.h"
#include "position.h"
#include "sudoku_functions.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(int argc, char **argv) {
  bool missing[SIZE];
  Scheme scheme;
  pos visualpos;
  char ch[2];
  pos highlight = {4, 4};
  pos conflicting = {-1, -1};
  mode md = INSERT;
  int hlnum = -1;
  char should = 0;
  char sudoku[SIZE][SIZE] = {0};
  bool notes[SIZE][SIZE][SIZE] = {false};
  bool cheats[SIZE][SIZE][SIZE];
  bool cheating = false;
  scheme.th = NORMAL;
  ch[1] = '\0';

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      for (int k = 0; k < SIZE; k++) {
        cheats[i][j][k] = true;
      }
    }
  }

  getSudokuFromCache(sudoku, "sudoku-cache");
  if (isValid(sudoku)) {
    calcCheats(cheats, sudoku);
  }
  updateColors(&scheme);

  InitWindow(500, 500, "Sudoku GUI");
  Font font = LoadFontEx("resources/montserrat.ttf", 256, 0, 1000);
  SetExitKey(KEY_BACKSPACE);
  SetTargetFPS(framerate);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(scheme.gaps);
    int num;

    switch (handleInput(&highlight.y, &highlight.x, GetCharPressed(), &md, &num,
                        &scheme)) {
    case 0: {
      switch (md) {
      case INSERT: {
        sudoku[highlight.y][highlight.x] = num;
        if (isValid(sudoku)) {
          if (cheating) {
            calcCheats(cheats, sudoku);
          }
          conflicting = (pos){-1, -1};
        } else {
          conflicting = highlight;
        }

        break;
      }
      case NOTE: {
        if (cheating) {
          cheats[highlight.y][highlight.x][num - 1] =
              !cheats[highlight.y][highlight.x][num - 1];
          break;
        }
        notes[highlight.y][highlight.x][num - 1] =
            !notes[highlight.y][highlight.x][num - 1];
        break;
      }
      case VISUAL: {
        for (int i = MIN(visualpos.y, highlight.y);
             i < MAX(visualpos.y, highlight.y) + 1; i++) {
          for (int j = MIN(visualpos.x, highlight.x);
               j < MAX(visualpos.x, highlight.x) + 1; j++) {
            if (!cheating) {
              notes[i][j][num - 1] = !notes[i][j][num - 1];
            } else {
              cheats[i][j][num - 1] = !cheats[i][j][num - 1];
            }
          }
        }
        break;
      }
      case MOUSEVISUAL: {
        for (int i = MIN(visualpos.y, highlight.y);
             i < MAX(visualpos.y, highlight.y) + 1; i++) {
          for (int j = MIN(visualpos.x, highlight.x);
               j < MAX(visualpos.x, highlight.x) + 1; j++) {
            if (!cheating) {
              notes[i][j][num - 1] = !notes[i][j][num - 1];
            } else {
              cheats[i][j][num - 1] = !cheats[i][j][num - 1];
            }
          }
        }
        md = INSERT;
        break;
      }
      case HIGHLIGHT: {
        hlnum = num;
        break;
      }
      default:
        should = 0;
        break;
      }
      break;
    }
    case -2: {
      visualpos = highlight;
      should = 0;
      break;
    }
    case -3: {
      char string[SIZE * SIZE + 1];
      for (int i = 0; i < SIZE * SIZE; i++) {
        string[i] = sudoku[i / 9][i % 9] + '0';
      }
      string[81] = '\0';
      SetClipboardText(string);
      if (should == 'r') {
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            sudoku[i][j] = 0;
            for (int k = 0; k < 9; k++) {
              notes[i][j][k] = false;
              cheats[i][j][k] = true;
            }
          }
        }
        cheating = false;
        conflicting = (pos){-1, -1};
        break;
      }
      should = 'r';
      break;
    }
    case -4: {
      if (should == 'c') {
        cheating = !cheating;
        calcCheats(cheats, sudoku);
        break;
      }
      should = 'c';
      break;
    }
    case -5: {
      if (GetClipboardText() == NULL) {
        break;
      }
      int dupe[SIZE][SIZE];
      if (strlen(GetClipboardText()) == 81) {
        int count = 0;
        for (int i = 0; i < SIZE * SIZE; i++) {
          int y = i / 9;
          int x = i % 9;
          if (sudoku[y][x] < 0 || sudoku[y][x] > 9) {
            for (int j = 0; j < count; j++) {
              sudoku[y][x] = dupe[y][x];
            }
            break;
          }
          dupe[y][x] = sudoku[y][x];
          sudoku[y][x] = GetClipboardText()[i] - '0';
        }
      }
      should = 0;
      break;
    }
    case -1: {
      should = 0;
      break;
    }
    default:
      break;
    }
    for (int y = 0; y < 9; y++) {
      for (int x = 0; x < 9; x++) {
        Color color = scheme.background;
        if (md != VISUAL && md != MOUSEVISUAL && md != HIGHLIGHT) {
          if (getBoxIndex(highlight.y, highlight.x) == getBoxIndex(y, x) ||
              (highlight.y == y || highlight.x == x)) {
            color = scheme.lowlight;
          }
          if ((y == highlight.y && x == highlight.x) ||
              (sudoku[highlight.y][highlight.x] == sudoku[y][x] &&
               sudoku[highlight.y][highlight.x] != 0)) {
            color = scheme.highlight;
          }
        } else if (md == VISUAL || md == MOUSEVISUAL) {
          if (y >= MIN(visualpos.y, highlight.y) &&
              x >= MIN(visualpos.x, highlight.x) &&
              y <= MAX(visualpos.y, highlight.y) &&
              x <= MAX(visualpos.x, highlight.x)) {
            color = scheme.lowlight;
          }
        } else if (md == HIGHLIGHT && hlnum == sudoku[y][x] &&
                   sudoku[y][x] != 0) {
          color = scheme.highlight;
        }
        if (conflicting.x == x && conflicting.y == y) {
          color = RED;
        }

        DrawRectangle(x * (500 / 9) + x, y * (500 / 9) + y, (500 / 9),
                      (500 / 9), color);
        ch[0] = sudoku[y][x] + '0';
        DrawTextEx(font, sudoku[y][x] == 0 ? " " : ch,
                   (Vector2){x * 55 + x +
                                 (27 - (MeasureTextEx(font, ch, 50, 2).x / 2)),
                             y * 55 + 4 + y},
                   50, 2, scheme.text);
        if (sudoku[y][x] == 0) {
          if (cheating) {
            for (int z = 0; z < 9; z++) {
              if (!cheats[y][x][z]) {
                continue;
              }
              char note[2] = {z + '1', '\0'};
              if (md == HIGHLIGHT && hlnum == note[0] - '0') {
                DrawRectangleV((Vector2){x * 55 + x + 18 * (z % 3) + 2,
                                         y * 55 + y + 18 * floor(z / 3.0f)},
                               Vector2Add(MeasureTextEx(font, note, 20, 0),
                                          (Vector2){4, 0}),
                               scheme.highlight);
              }
              DrawTextEx(font, note,
                         (Vector2){x * 55 + x + 18 * (z % 3) + 4,
                                   y * 55 + y + 18 * floor(z / 3.0f)},
                         20, 0, scheme.notes);
            }
          } else {
            for (int z = 0; z < 9; z++) {
              if (!notes[y][x][z]) {
                continue;
              }
              char note[2] = {z + '1', '\0'};
              if (md == HIGHLIGHT && hlnum == note[0] - '0') {
                DrawRectangleV((Vector2){x * 55 + x + 18 * (z % 3) + 2,
                                         y * 55 + y + 18 * floor(z / 3.0f)},
                               Vector2Add(MeasureTextEx(font, note, 20, 0),
                                          (Vector2){4, 0}),
                               scheme.lowlight);
              }
              DrawTextEx(font, note,
                         (Vector2){x * 55 + x + 18 * (z % 3) + 4,
                                   y * 55 + y + 18 * floor(z / 3.0f)},
                         20, 0, scheme.notes);
            }
          }
        }
      }
    }
    for (int i = 0; i < 2; i++) {
      DrawRectangle(0, (500 / 3) + (500 / 3) * i + (i * 2), 500, 3,
                    scheme.gaps);
    }
    for (int i = 0; i < 2; i++) {
      DrawRectangle((500 / 3) + (500 / 3) * i + (i * 2), 0, 3, 500,
                    scheme.gaps);
    }

    switch (md) {
    case MISSING: {
      Vector2 offset = (Vector2){floor(500 / 2.0) - floor(100 / 2.0),
                                 floor(500 / 2.0) - floor(70 / 2.0)};
      DrawRectangle(offset.x - 1, offset.y - 1, 104 + 1, 74 + 1, scheme.text);
      DrawRectangle(offset.x + 1, offset.y + 1, 100 + 1, 70 + 1,
                    scheme.background);

      offset.x += 9;
      offset.y += 4;

      int defaultX = offset.x;

      char ch[2] = {5, '\0'};

      DrawTextEx(font, "B: ", (Vector2){offset.x, offset.y}, 15, 0,
                 scheme.text);

      offset.x += MeasureTextEx(font, "B: ", 15, 2).x;

      getMissingBox(highlight.y, highlight.x, missing, sudoku);

      for (int i = 0; i < SIZE; i++) {
        ch[0] = missing[i] == true ? i + '1' : ' ';
        DrawTextEx(font, ch, offset, 15, 3, scheme.text);
        offset.x += strcmp(" ", ch) == 0 ? 0 : MeasureTextEx(font, ch, 15, 3).x;
      }

      offset.y += MeasureTextEx(font, ch, 15, 3).y + 9;
      offset.x = defaultX;

      DrawTextEx(font, "R: ", (Vector2){offset.x, offset.y}, 15, 0,
                 scheme.text);

      offset.x += MeasureTextEx(font, "R: ", 15, 2).x;

      getMissingRow(highlight.y, missing, sudoku);

      for (int i = 0; i < SIZE; i++) {
        ch[0] = missing[i] == true ? i + '1' : ' ';
        DrawTextEx(font, ch, offset, 15, 3, scheme.text);
        offset.x += strcmp(" ", ch) == 0 ? 0 : MeasureTextEx(font, ch, 15, 3).x;
      }

      offset.y += MeasureTextEx(font, ch, 15, 3).y + 9;
      offset.x = defaultX;

      DrawTextEx(font, "C: ", (Vector2){offset.x, offset.y}, 15, 0,
                 scheme.text);

      offset.x += MeasureTextEx(font, "C: ", 15, 2).x;

      getMissingCol(highlight.x, missing, sudoku);

      for (int i = 0; i < SIZE; i++) {
        ch[0] = missing[i] == true ? i + '1' : ' ';
        DrawTextEx(font, ch, offset, 15, 3, scheme.text);
        offset.x += strcmp(" ", ch) == 0 ? 0 : MeasureTextEx(font, ch, 15, 3).x;
      }
    }
    default:
      break;
    }
    EndDrawing();
  }
  UnloadFont(font);
  CloseWindow();
  cacheSudoku(sudoku, "sudoku-cache");
}
