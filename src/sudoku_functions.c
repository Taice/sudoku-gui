#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#include "sudoku_functions.h"
bool findEmptySpace(int sudoku[SIZE][SIZE], int *row, int *col);

short isWinnable(int sudoku[SIZE][SIZE])
{
    int clone[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            clone[i][j] = sudoku[i][j];
        }
    }

    int row, col;

    if (isValid(clone))
    {
        if (!findEmptySpace(clone, &row, &col))
        {
            return 0;
        }
        if (backtrack(clone))
        {
            return 1;
        }
    }

    return -1;
}

bool isValid(int sudoku[SIZE][SIZE])
{
    bool rows[SIZE][SIZE] = {false};
    bool cols[SIZE][SIZE] = {false};
    bool boxes[SIZE][SIZE] = {false};

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (sudoku[i][j] != 0)
            {
                int num = sudoku[i][j] - 1;
                int box_index = getBoxIndex(i, j);

                if (rows[i][num] || cols[j][num] || boxes[box_index][num])
                {
                    return false;
                }

                rows[i][num] = true;
                cols[j][num] = true;
                boxes[box_index][num] = true;
            }
        }
    }
    return true;
}

bool findEmptySpace(int sudoku[SIZE][SIZE], int *row, int *col)
{
    for (*row = 0; *row < SIZE; (*row)++)
    {
        for (*col = 0; *col < SIZE; (*col)++)
        {
            if (sudoku[*row][*col] == 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool backtrack(int sudoku[SIZE][SIZE])
{
    int row, col;

    if (!findEmptySpace(sudoku, &row, &col))
    {
        return true;
    }

    for (int num = 1; num <= SIZE; num++)
    {
        sudoku[row][col] = num;

        if (isValid(sudoku))
        {
            if (backtrack(sudoku))
            {
                return true;
            }
        }

        sudoku[row][col] = 0;
    }
    return false;
}

int getBoxIndex(int row, int col)
{
    return (row / SUBGRID_SIZE) * SUBGRID_SIZE + col / SUBGRID_SIZE;
}

void getMissingBox(int row, int col, bool missing[SIZE], int sudoku[SIZE][SIZE])
{
    int t_row = floor(row / 3.0) * 3;
    int t_col = floor(col / 3.0) * 3;
    for (int num = 0; num < SIZE; num++)
    {
        missing[num] = true;
        for (int i = t_row; i < t_row + 3; i++)
        {
            for (int j = t_col; j < t_col + 3; j++)
            {
                if (sudoku[i][j] == num + 1)
                {
                    missing[num] = false;
                }
            }
        }
    }
}

void getMissingCol(int col, bool missing[SIZE], int sudoku[SIZE][SIZE])
{
    for (int num = 0; num < SIZE; num++)
    {
        missing[num] = true;
        for (int row = 0; row < SIZE; row++)
        {
            if (sudoku[row][col] == num + 1)
            {
                missing[num] = false;
            }
        }
    }
}

void getMissingRow(int row, bool missing[SIZE], int sudoku[SIZE][SIZE])
{

    for (int num = 0; num < SIZE; num++)
    {
        missing[num] = true;
        for (int col = 0; col < SIZE; col++)
        {
            if (sudoku[row][col] == num + 1)
            {
                missing[num] = false;
            }
        }
    }
}

void calcCheats(bool cheats[SIZE][SIZE][SIZE], int sudoku[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            bool missing[] = {true, true, true, true, true,
                              true, true, true, true};
            for (int num = 0; num < SIZE; num++)
            {
                for (int col = 0; col < SIZE; col++)
                {
                    if (sudoku[i][col] == num + 1)
                    {
                        missing[num] = false;
                    }
                }
            }
            for (int num = 0; num < SIZE; num++)
            {
                for (int row = 0; row < SIZE; row++)
                {
                    if (sudoku[row][j] == num + 1)
                    {
                        missing[num] = false;
                    }
                }
            }
            int t_row = floor(i / 3.0) * 3;
            int t_col = floor(j / 3.0) * 3;
            for (int num = 0; num < SIZE; num++)
            {
                for (int i = t_row; i < t_row + 3; i++)
                {
                    for (int j = t_col; j < t_col + 3; j++)
                    {
                        if (sudoku[i][j] == num + 1)
                        {
                            missing[num] = false;
                        }
                    }
                }
            }
            for (int k = 0; k < SIZE; k++)
            {
                cheats[i][j][k] = false;
                if (missing[k])
                {
                    cheats[i][j][k] = true;
                }
            }
        }
    }
}
