#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "sudoku_functions.h"
bool findEmptySpace(char sudoku[SIZE][SIZE], int *row, int *col);

short isWinnable(char sudoku[SIZE][SIZE]) {
  char clone[SIZE][SIZE];

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      clone[i][j] = sudoku[i][j];
    }
  }

  int row, col;

  bool solution = false;
  if (isValid(clone)) {
    if (!findEmptySpace(clone, &row, &col)) {
      return 0;
    }
    int result = backtrack(clone, &solution);
    if (result == -1) {
      return -2;
    } else if (result == 1) {
      return 1;
    }
  }
  return -1;
}

bool whereIsValid(char sudoku[SIZE][SIZE], int *row, int *col) {
  bool rows[SIZE][SIZE] = {false};
  bool cols[SIZE][SIZE] = {false};
  bool boxes[SIZE][SIZE] = {false};

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (sudoku[i][j] != 0) {
        int num = sudoku[i][j] - 1;
        int box_index = getBoxIndex(i, j);

        if (rows[i][num] || cols[j][num] || boxes[box_index][num]) {
          (*row) = i;
          (*col) = j;
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
bool isValid(char sudoku[SIZE][SIZE]) {
  bool rows[SIZE][SIZE] = {false};
  bool cols[SIZE][SIZE] = {false};
  bool boxes[SIZE][SIZE] = {false};

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (sudoku[i][j] != 0) {
        int num = sudoku[i][j] - 1;
        int box_index = getBoxIndex(i, j);

        if (rows[i][num] || cols[j][num] || boxes[box_index][num]) {
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

bool findEmptySpace(char sudoku[SIZE][SIZE], int *row, int *col) {
  for (*row = 0; *row < SIZE; (*row)++) {
    for (*col = 0; *col < SIZE; (*col)++) {
      if (sudoku[*row][*col] == 0) {
        return true;
      }
    }
  }
  return false;
}

int backtrack(char sudoku[SIZE][SIZE], bool *foundSolution) {
  int row, col;

  if (!findEmptySpace(sudoku, &row, &col)) {
    if (*foundSolution) {
      return -1;

    } else {
      *foundSolution = true;
      return 1;
    }
  }

  for (int num = 1; num <= SIZE; num++) {
    sudoku[row][col] = num;

    if (isValid(sudoku)) {
      int result = backtrack(sudoku, foundSolution);
      if (result == -1) {
        return -1;
      }
      if (result == 1 && num == SIZE) {
        return 1;
      }
    }

    sudoku[row][col] = 0;
  }
  return 0;
}

int getBoxIndex(int row, int col) {
  return (row / SUBGRID_SIZE) * SUBGRID_SIZE + col / SUBGRID_SIZE;
}

void getMissingBox(int row, int col, bool missing[SIZE],
                   char sudoku[SIZE][SIZE]) {
  int t_row = floor(row / 3.0) * 3;
  int t_col = floor(col / 3.0) * 3;
  for (int num = 0; num < SIZE; num++) {
    missing[num] = true;
    for (int i = t_row; i < t_row + 3; i++) {
      for (int j = t_col; j < t_col + 3; j++) {
        if (sudoku[i][j] == num + 1) {
          missing[num] = false;
        }
      }
    }
  }
}

void getMissingCol(int col, bool missing[SIZE], char sudoku[SIZE][SIZE]) {
  for (int num = 0; num < SIZE; num++) {
    missing[num] = true;
    for (int row = 0; row < SIZE; row++) {
      if (sudoku[row][col] == num + 1) {
        missing[num] = false;
      }
    }
  }
}

void getMissingRow(int row, bool missing[SIZE], char sudoku[SIZE][SIZE]) {

  for (int num = 0; num < SIZE; num++) {
    missing[num] = true;
    for (int col = 0; col < SIZE; col++) {
      if (sudoku[row][col] == num + 1) {
        missing[num] = false;
      }
    }
  }
}

void calcCheats(bool cheats[SIZE][SIZE][SIZE], char sudoku[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      bool missing[SIZE];
      for (int k = 0; k < SIZE; k++) {
        missing[k] = cheats[i][j][k];
      }
      for (int num = 0; num < SIZE; num++) {
        for (int col = 0; col < SIZE; col++) {
          if (sudoku[i][col] == num + 1) {
            missing[num] = false;
          }
        }
      }
      for (int num = 0; num < SIZE; num++) {
        for (int row = 0; row < SIZE; row++) {
          if (sudoku[row][j] == num + 1) {
            missing[num] = false;
          }
        }
      }
      int t_row = floor(i / 3.0) * 3;
      int t_col = floor(j / 3.0) * 3;
      for (int num = 0; num < SIZE; num++) {
        for (int i = t_row; i < t_row + 3; i++) {
          for (int j = t_col; j < t_col + 3; j++) {
            if (sudoku[i][j] == num + 1) {
              missing[num] = false;
            }
          }
        }
      }
      for (int k = 0; k < SIZE; k++) {
        cheats[i][j][k] = false;
        if (missing[k]) {
          cheats[i][j][k] = true;
        }
      }
    }
  }
}

void cacheSudoku(char sudoku[SIZE][SIZE], char *sudokufile) {
  remove(sudokufile);

  FILE *cache = fopen(sudokufile, "w");
  if (cache == NULL) {
    fprintf(stderr, "error opening sudokufile\n");
    return;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      fprintf(cache, "%c", sudoku[i][j] + '0');
    }
  }

  fclose(cache);
}

void cacheNotes(bool notes[SIZE][SIZE][SIZE], char *notefile) {
  FILE *cache = fopen(notefile, "w");
  if (cache == NULL) {
    fprintf(stderr, "error opening notefile\n");
    return;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      for (int k = 0; k < SIZE; k++) {
        fprintf(cache, "%c", notes[i][j][k] ? '1' : '0');
      }
    }
  }

  fclose(cache);
}

int getSudokuFromCache(char sudoku[SIZE][SIZE], char *filename) {
  FILE *cache = fopen(filename, "r");
  if (cache == NULL) {
    return -1;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      char buffer = fgetc(cache);
      if (buffer == EOF) {
        fclose(cache);
        return -1;
      }
      sudoku[i][j] = buffer - '0';
    }
  }

  fclose(cache);
}

void getNotesFromCache(bool notes[SIZE][SIZE][SIZE], char *filename) {
  FILE *cache = fopen(filename, "r");
  if (cache == NULL) {
    return;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      for (int k = 0; k < SIZE; k++) {
        char buffer = fgetc(cache);
        if (buffer == EOF) {
          fclose(cache);
          return;
        }
        notes[i][j][k] = (buffer == '1');
      }
    }
  }

  fclose(cache);
}
