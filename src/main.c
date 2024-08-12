#include "color.h"
#include "framerate.h"
#include "input.h"
#include "mode.h"
#include "position.h"
#include "sudoku_functions.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(int argc, char **argv) {
  bool missing[SIZE];
  Scheme scheme;
  pos startpos;
  char ch[2];
  pos highlight = {4, 4};
  mode md = INSERT;
  char should = 0;
  char sudoku[SIZE][SIZE] = {0};
  bool notes[SIZE][SIZE][SIZE] = {false};
  bool cheats[SIZE][SIZE][SIZE] = {false};
  bool cheating = false;
  bool cheatable = !(argc == 1);
  scheme.th = NORMAL;
  ch[1] = '\0';

  calcCheats(cheats, sudoku);
  updateColors(&scheme);

  InitWindow(500, 500, "Sudoku GUI");
  Font font = LoadFontEx("resources/montserrat.ttf", 256, 0, 1000);
  SetTargetFPS(framerate);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(scheme.gaps);
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        Color color = scheme.background;
        if (md != VISUAL) {
          if (getBoxIndex(highlight.y, highlight.x) == getBoxIndex(i, j) ||
              (highlight.y == i || highlight.x == j)) {
            color = scheme.lowlight;
          }
          if ((highlight.y == i && highlight.x == j) ||
              (sudoku[highlight.y][highlight.x] == sudoku[i][j] &&
               sudoku[highlight.y][highlight.x] != 0)) {
            color = scheme.highlight;
          }
        } else {
          if (i >= MIN(startpos.y, highlight.y) &&
              j >= MIN(startpos.x, highlight.x) &&
              i <= MAX(startpos.y, highlight.y) &&
              j <= MAX(startpos.x, highlight.x)) {
            color = scheme.lowlight;
          }
        }

        DrawRectangle(j * (500 / 9) + j, i * (500 / 9) + i, (500 / 9),
                      (500 / 9), color);
        ch[0] = sudoku[i][j] + '0';
        DrawTextEx(font, sudoku[i][j] == 0 ? " " : ch,
                   (Vector2){j * 55 + j +
                                 (27 - (MeasureTextEx(font, ch, 50, 2).x / 2)),
                             i * 55 + 4 + i},
                   50, 2, scheme.text);
        if (sudoku[i][j] == 0) {
          if (cheating) {
            for (int k = 0; k < 9; k++) {
              if (!cheats[i][j][k]) {
                continue;
              }
              char note[2] = {k + '1', '\0'};
              DrawTextEx(font, note,
                         (Vector2){j * 55 + j + 18 * (k % 3) + 4,
                                   i * 55 + i + 18 * floor(k / 3.0f)},
                         20, 0, scheme.notes);
            }
          } else {
            for (int k = 0; k < 9; k++) {
              if (!notes[i][j][k]) {
                continue;
              }
              char note[2] = {k + '1', '\0'};
              DrawTextEx(font, note,
                         (Vector2){j * 55 + j + 18 * (k % 3) + 4,
                                   i * 55 + i + 18 * floor(k / 3.0f)},
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
    int num;

    switch (
        handleInput(&highlight.y, &highlight.x, GetCharPressed(), &md, &num, &scheme)) {
    case 0: {
      switch (md) {
      case INSERT: {
        int dfault = sudoku[highlight.y][highlight.x];
        sudoku[highlight.y][highlight.x] = num;
        if (isWinnable(sudoku) == -1) {
          sudoku[highlight.y][highlight.x] = dfault;
        } else if (isWinnable(sudoku) == 0) {
          return 0;
        }
        calcCheats(cheats, sudoku);
        break;
      }
      case NOTE:
        if (cheating) {
          cheats[highlight.y][highlight.x][num - 1] =
              !cheats[highlight.y][highlight.x][num - 1];
          break;
        }
        notes[highlight.y][highlight.x][num - 1] =
            !notes[highlight.y][highlight.x][num - 1];
        break;
      case VISUAL:
        for (int i = MIN(startpos.y, highlight.y);
             i < MAX(startpos.y, highlight.y) + 1; i++) {
          for (int j = MIN(startpos.x, highlight.x);
               j < MAX(startpos.x, highlight.x) + 1; j++) {
            notes[i][j][num - 1] = !notes[i][j][num - 1];
          }
        }
        break;
      default:
        break;
      }
      should = 0;
      break;
    }
    case -2: {
      startpos = highlight;
      should = 0;
      break;
    }
    case -3: {
      if (should == 'r') {
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            sudoku[i][j] = 0;
            for (int k = 0; k < 9; k++) {
              notes[i][j][k] = false;
            }
          }
        }
        should = 0;
        break;
      }
      should = 'r';
      break;
    }
    case -4: {
      if (should == 'c') {
        cheating = cheatable ? !cheating : false;
        break;
      }
      should = 'c';
      break;
    }

    case -1: {
      should = 0;
      break;
    }
    default:
      break;
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

      DrawTextEx(font, "C: ", (Vector2){offset.x, offset.y}, 15, 0, scheme.text);

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
}
