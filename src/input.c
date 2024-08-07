#include "input.h"

int handleInput(int *row, int *col, char ch, mode *md)
{
    if (ch == 'h')
    {
        if (*col > 0)
        {
            (*col)--;
            return -1;
        }
        return -1;
    }
    if (ch == 'l')
    {
        if (*col < 8)
        {
            (*col)++;
            return -1;
        }
        return -1;
    }
    if (ch == 'j')
    {
        if (*row < 8)
        {
            (*row)++;
            return -1;
        }
        return -1;
    }
    if (ch == 'k')
    {
        if (*row > 0)
        {
            (*row)--;
            return -1;
        }
        return -1;
    }
    if (ch > '0' && ch <= '9')
    {
        return ch - '0';
    }

    if (ch == 'n')
    {
        (*md) = NOTE;
        return -1;
    }
    if (ch == 'i')
    {
        (*md) = INSERT;
        return -1;
    }
    if (ch == 'v')
    {
        (*md) = VISUAL;
        return -2;
    }

    return -1;
}
