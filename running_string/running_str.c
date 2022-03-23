#include "running_str.h"
#include "letters.h"

#include <string.h>
#include <stdlib.h>

void *RS_LetterGet(char c);
void  RS_DisplayProc(uint8_t *letter);

void *RS_LetterGet(char c)
{
    switch (c)
    {
        case 'a': case 'A': return letterA;
        case 'b': case 'B': return letterB;
        case 'c': case 'C': return letterC;
        case 'd': case 'D': return letterD;
        case 'e': case 'E': return letterE;
        case 'f': case 'F': return letterF;
        case 'g': case 'G': return letterG;
        case 'h': case 'H': return letterH;
        case 'i': case 'I': return letterI;
        case 'j': case 'J': return letterJ;
        case 'k': case 'K': return letterK;
        case 'l': case 'L': return letterL;
        case 'm': case 'M': return letterM;
        case 'n': case 'N': return letterN;
        case 'o': case 'O': return letterO;
        case 'p': case 'P': return letterP;
        case 'q': case 'Q': return letterQ;
        case 'r': case 'R': return letterR;
        case 's': case 'S': return letterS;
        case 't': case 'T': return letterT;
        case 'u': case 'U': return letterU;
        case 'v': case 'V': return letterV;
        case 'w': case 'W': return letterW;
        case 'x': case 'X': return letterX;
        case 'y': case 'Y': return letterY;
        case 'z': case 'Z': return letterZ;

        case '0': return letter0;
        case '1': return letter1;
        case '2': return letter2;
        case '3': return letter3;
        case '4': return letter4;
        case '5': return letter5;
        case '6': return letter6;
        case '7': return letter7;
        case '8': return letter8;
        case '9': return letter9;

        default: return NULL;
    }
}

static uint8_t displayMatrix[RS_DISPLAY_HEIGHT];

char *running_str = NULL;
char *current_sym = NULL;

int RS_StringSet(char *str)
{
    if (running_str)
        return -1;

    if (!str || !strlen(str))
        return -2;

    running_str = malloc(strlen(str) + 1);

    strcpy(running_str, str);

    current_sym = running_str;

    return 0;
}


void *RS_StringProc()
{
    if (!running_str || !current_sym)
        return NULL;

    if (*current_sym == '\0')
    {
        free(running_str);

        running_str = current_sym = NULL;

        return NULL;
    }

    RS_DisplayProc(RS_LetterGet(*current_sym));

    current_sym++;

    return displayMatrix;
}

void RS_DisplayProc(uint8_t *letter)
{
    int i, j;
    int padding_horizontal = 1;

    if (!letter)
    {
        memset(displayMatrix, 0, sizeof(displayMatrix));
        return;
    }

    for (i = RS_DISPLAY_PADDING_VERTICAL; i < RS_DISPLAY_HEIGHT; i++)
    {
        j = i - RS_DISPLAY_PADDING_VERTICAL;

        if (j < LETTER_HEIGHT)
        {
            displayMatrix[i] = letter[j] << padding_horizontal;
        }
    }
}


