#include "input.h"
#include "mode.h"
#include "position.h"
#include "sudoku_functions.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <unistd.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main()
{
    pos highlight = {4, 4};
    pos startpos;
    int sudoku[SIZE][SIZE] = {0};
    bool notes[SIZE][SIZE][SIZE] = {false};
    char ch[2];
    ch[1] = '\0';

    mode md = INSERT;

    InitWindow(500, 500, "Sudoku GUI");
    Font font = LoadFontEx("resources/montserrat.ttf", 256, 0, 1000);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                Color color = RAYWHITE;
                if (md != VISUAL)
                {
                    if (getBoxIndex(highlight.y, highlight.x) ==
                            getBoxIndex(i, j) ||
                        (highlight.y == i || highlight.x == j) ||
                        (sudoku[highlight.y][highlight.x] == sudoku[i][j] &&
                         sudoku[highlight.y][highlight.x] != 0))
                    {
                        color = CLITERAL(Color){172, 186, 250, 255};
                    }
                    if (highlight.y == i && highlight.x == j)
                    {
                        color = CLITERAL(Color){134, 151, 255, 255};
                    }
                }
                else
                {
                    if (i >= MIN(startpos.y, highlight.y) &&
                        j >= MIN(startpos.x, highlight.x) &&
                        i <= MAX(startpos.y, highlight.y) &&
                        j <= MAX(startpos.x, highlight.x))
                    {
                        color = CLITERAL(Color){172, 186, 250, 255};
                    }
                }

                DrawRectangle(
                    j * (500 / 9) + j, i * (500 / 9) + i, (500 / 9), (500 / 9),
                    color);
                ch[0] = sudoku[i][j] + '0';
                DrawTextEx(
                    font, sudoku[i][j] == 0 ? " " : ch,
                    CLITERAL(Vector2){
                        j * 55 + j +
                            (27 - (MeasureTextEx(font, ch, 50, 2).x / 2)),

                        i * 55 + 4 + i},
                    50, 2, BLACK);
                if (sudoku[i][j] == 0)
                {
                    for (int k = 0; k < 9; k++)
                    {
                        if (!notes[i][j][k])
                        {
                            continue;
                        }
                        char note[2] = {k + '1', '\0'};
                        DrawTextEx(
                            font, note,
                            CLITERAL(Vector2){
                                j * 55 + j + 18 * (k % 3) + 4,
                                i * 55 + i + 18 * floor(k / 3.0f)},
                            20, 0, DARKGRAY);
                    }
                }
            }
        }
        for (int i = 0; i < 2; i++)
        {
            DrawRectangle(
                0, (500 / 3) + (500 / 3) * i + (i * 2), 500, 3, BLACK);
        }
        for (int i = 0; i < 2; i++)
        {
            DrawRectangle(
                (500 / 3) + (500 / 3) * i + (i * 2), 0, 3, 500, BLACK);
        }
        int num =
            handleInput(&highlight.y, &highlight.x, GetCharPressed(), &md);

        if (num == -2)
        {
            startpos = highlight;
        }

        if (num != -1 && num != -2)
        {
            switch (md)
            {
            case INSERT:
            {
                int dfault = sudoku[highlight.y][highlight.x];
                sudoku[highlight.y][highlight.x] = num;
                if (isWinnable(sudoku) == -1)
                {
                    sudoku[highlight.y][highlight.x] = dfault;
                }
                else if (isWinnable(sudoku) == 0)
                {
                    return 0;
                }
                break;
            }
            case NOTE:
                notes[highlight.y][highlight.x][num - 1] =
                    !notes[highlight.y][highlight.x][num - 1];
                break;
            case VISUAL:
                for (int i = MIN(startpos.y, highlight.y);
                     i < MAX(startpos.y, highlight.y) + 1; i++)
                {
                    for (int j = MIN(startpos.x, highlight.x);
                         j < MAX(startpos.x, highlight.x) + 1; j++)
                    {
                        notes[i][j][num - 1] = !notes[i][j][num - 1];
                    }
                }
                break;
            }
        }
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
}
