#include "running_str.h"
#include "letters.h"

#include <string.h>
#include <stdlib.h>

void *RS_LetterGet(char c);
int   RS_DisplayProc(uint8_t *cur_letter, uint8_t *next_letter);

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

        case '!': return letterEXCLAMATION;
        case '?': return letterQUESTION;
        case '-': return letterDASH;

        default: return NULL;
    }
}

static uint8_t displayMatrix[RS_DISPLAY_HEIGHT];

char *running_str = NULL;
char *current_sym = NULL;
char *next_sym    = NULL;

int cur_sym_pos;
int next_sym_pos;

int RS_StringSet(char *str)
{
    if (running_str)
        return -1;

    if (!str || !strlen(str))
        return -2;

    running_str = (char *)malloc(strlen(str) + 2);

    strcpy(running_str, str);
    strcat(running_str, " ");

    memset(displayMatrix, 0, sizeof(displayMatrix));

    current_sym = running_str;
    next_sym = current_sym + 1;

    cur_sym_pos = RS_DISPLAY_WIDTH + 1;
    next_sym_pos = cur_sym_pos + LETTER_WIDTH + 1;

    return 0;
}

void RS_StringStop()
{
    if (!running_str || !current_sym)
        return;

    *current_sym = '\0';

    RS_StringProc();
}

void *RS_StringProc()
{
    int cur_sym_left_display;

    if (!running_str || !current_sym)
        return NULL;

    if (*current_sym == '\0')
    {
        free(running_str);

        running_str = current_sym = NULL;

        return NULL;
    }

    cur_sym_left_display = RS_DisplayProc((uint8_t *)RS_LetterGet(*current_sym), (uint8_t *)RS_LetterGet(*next_sym));

    if (cur_sym_left_display)
    {
        current_sym = next_sym;
        next_sym++;

        cur_sym_pos = --next_sym_pos;
        next_sym_pos = cur_sym_pos + LETTER_WIDTH + 1;
    } else {
        cur_sym_pos--;
        next_sym_pos--;
    }

    return displayMatrix;
}

int RS_DisplayProc(uint8_t *cur_letter, uint8_t *next_letter)
{
    int i, j, res = 0;
    // int padding_horizontal = 1;
    int shift;

    // if (!cur_letter)
    // {
    //     memset(displayMatrix, 0, sizeof(displayMatrix));
    //     return res;
    // }

    if (cur_sym_pos <= 1 - LETTER_WIDTH)
    {
        /*  */
        res = 1;
    }

    for (i = RS_DISPLAY_PADDING_VERTICAL; i < RS_DISPLAY_HEIGHT; i++)
    {
        j = i - RS_DISPLAY_PADDING_VERTICAL;

        if (j < LETTER_HEIGHT)
        {
            shift = (RS_DISPLAY_WIDTH - cur_sym_pos) - LETTER_WIDTH;

            if (cur_letter)
            {
                if (shift >= 0)
                {
                    displayMatrix[i] = cur_letter[j] << shift;
                } else {
                    displayMatrix[i] = cur_letter[j] >> -shift;
                }
            } else {
                // memset(displayMatrix, 0, sizeof(displayMatrix));
                displayMatrix[i] = 0;
            }

            if (next_letter)
            {
                shift = (RS_DISPLAY_WIDTH - next_sym_pos) - LETTER_WIDTH;

                if (shift >= 0)
                {
                    displayMatrix[i] |= next_letter[j] << shift;
                } else {
                    displayMatrix[i] |= next_letter[j] >> -shift;
                }
            }

            // displayMatrix[i] = cur_letter[j] << padding_horizontal;
        }
    }

    return res;
}


