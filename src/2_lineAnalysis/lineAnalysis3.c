#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned int uint;
int s[2][16] = {{0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7},{0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7}};
int p[65536] = {0};


// 输入输出优化
char inputBuffer[1 << 20], *inputPointer1, *inputPointer2;
#define mygetchar() (inputPointer1 == inputPointer2 && (inputPointer2 = (inputPointer1 = inputBuffer) + fread(inputBuffer, 1, 1 << 20, stdin), inputPointer1 == inputPointer2) ? EOF : *inputPointer1++)
char outputBuffer[1 << 22], *outputPointer = outputBuffer;
#define myflush() fwrite(outputBuffer, 1, outputPointer - outputBuffer, stdout), outputPointer = outputBuffer
#define myputchar(c) (*outputPointer++ = (c), outputPointer == outputBuffer + (1 << 22) ? myflush() : 0)
#define isDigitChar(c) ((c) >= '0' && (c) <= '9')
#define isHexChar(c) ((c) != ' ' && (c) != '\n')
#define isdigit(c) ((c) >= 0 && (c) <= 9)

uint read()
{
    int x = 0;
    char c = mygetchar();
    while (!isHexChar(c))
    {
        /* code */
        c = mygetchar();
    }
    do
    {
        /* code */
        x = isDigitChar(c) ? ((x << 4) + c - '0') : ((x << 4) + c - 'a' + 10);
        c = mygetchar();
    } while (isHexChar(c));
    
    
}




