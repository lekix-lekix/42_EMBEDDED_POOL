#include "./include.h"

uint8_t char_to_seg(char c) {
    switch (c) {
        case '0': return SEG_0;
        case '1': return SEG_1;
        case '2': return SEG_2;
        case '3': return SEG_3;
        case '4': return SEG_4;
        case '5': return SEG_5;
        case '6': return SEG_6;
        case '7': return SEG_7;
        case '8': return SEG_8;
        case '9': return SEG_9;

        case 'A': return SEG_A;
        case 'B': return SEG_B;
        case 'C': return SEG_C;
        case 'D': return SEG_D;
        case 'E': return SEG_E;
        case 'F': return SEG_F;
        case 'G': return SEG_G;
        case 'H': return SEG_H;
        case 'I': return SEG_I;
        case 'J': return SEG_J;
        case 'L': return SEG_L;
        case 'N': return SEG_N;
        case 'O': return SEG_O;
        case 'P': return SEG_P;
        case 'Q': return SEG_Q;
        case 'R': return SEG_R;
        case 'S': return SEG_S;
        case 'T': return SEG_T;
        case 'U': return SEG_U;
        case 'V': return SEG_V;
        case 'Y': return SEG_Y;
        case 'Z': return SEG_Z;

        default: return 0x00; // Rien si inconnu
    }
}