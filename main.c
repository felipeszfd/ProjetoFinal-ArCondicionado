#include <pic18f4520.h>
#include "config.h"
#include "bits.h"
#include "ssd.h"
#include "keypad.h"
#include "lcd.h"
#include "funcoes.h"
#include "atraso.h"
#include "adc.h"

#define L_L1 0x80
#define L_L2 0xC0
#define L_L3 0x90
#define L_L4 0xD0
#define L_CLR 0x01
#define ON 0x0F

int main() {
    char logo1[32] = {
        0x00, 0x00, 0x00, 0x18, 0x1C, 0x0E, 0x07, 0x03,
        0x00, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x1C, 0x18,
        0x01, 0x03, 0x07, 0x0F, 0x1D, 0x19, 0x01, 0x03,
        0x10, 0x18, 0x1C, 0x1E, 0x17, 0x13, 0x10, 0x18
    };
    char logo2[32] = {
        0x00, 0x00, 0x00, 0x08, 0x0D, 0x07, 0x04, 0x0C,
        0x00, 0x04, 0x0C, 0x1C, 0x19, 0x0B, 0x0E, 0x06,
        0x0E, 0x1C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x03,
        0x06, 0x03, 0x01, 0x03, 0x03, 0x06, 0x0C, 0x18
    };
    char logo3[32] = {
        0x00, 0x00, 0x00, 0x01, 0x09, 0x05, 0x03, 0x1F,
        0x00, 0x00, 0x00, 0x10, 0x12, 0x14, 0x18, 0x1F,
        0x1F, 0x03, 0x05, 0x09, 0x01, 0x00, 0x00, 0x00,
        0x1F, 0x18, 0x14, 0x12, 0x10, 0x00, 0x00, 0x00
    };

    unsigned int temp = 16;
    char i = 0;
    float j;
    int op = 0;

    lcdInit();
    kpInit();

    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdData('A');
    lcdData('r');
    lcdData(' ');
    lcdData('C');
    lcdData('o');
    lcdData('n');
    lcdData('d');
    lcdData('i');
    lcdData('c');
    lcdData('i');
    lcdData('o');
    lcdData('n');
    lcdData('a');
    lcdData('d');
    lcdData('o');
    atraso_ms(1000);
    lcdCommand(L_L2);
    lcdData('I');
    lcdData('n');
    lcdData('i');
    lcdData('c');
    lcdData('i');
    lcdData('a');
    lcdData('n');
    lcdData('d');
    lcdData('o');
    lcdData('.');
    lcdData('.');
    lcdData('.');

    atraso_ms(1000);
    ConfiguraLed();
    for (i = 0; i < 8; i++) {
        LigarLed(i);
        for (j = 0; j < 1000; j++);
    }
    for (i = 0; i < 8; i++) {
        DesligarLed(i);
        for (j = 0; j < 1000; j++);
    }

    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdData('O');
    lcdData('p');
    lcdData('c');
    lcdData('o');
    lcdData('e');
    lcdData('s');
    lcdData(':');
    lcdCommand(L_L2);
    lcdData(' ');
    lcdData('1');
    lcdData('-');
    lcdData('F');
    lcdData('a');
    lcdData('n');
    lcdCommand(L_L3);
    lcdData(' ');
    lcdData('2');
    lcdData('-');
    lcdData('H');
    lcdData('e');
    lcdData('a');
    lcdData('t');
    lcdCommand(L_L4);
    lcdData(' ');
    lcdData('3');
    lcdData('-');
    lcdData('C');
    lcdData('o');
    lcdData('o');
    lcdData('l');

    lcdCommand(ON);

    for (;;) {
        kpDebounce();
        if (kpRead() != temp) {
            temp = kpRead();
            if (bitTst(temp, 3)) {
                lcdCommand(L_CLR);
                lcdCommand(L_L1 + 2);
                lcdData('F');
                lcdData('a');
                lcdData('n');
                lcdData(':');
                imprimeLogo(logo1);
                op = 1;
                break;
            }
            if (bitTst(temp, 7)) {
                lcdCommand(L_CLR);
                lcdCommand(L_L1 + 2);
                lcdData('H');
                lcdData('e');
                lcdData('a');
                lcdData('t');
                lcdData(':');
                imprimeLogo(logo2);
                op = 2;
                break;
            }
            if (bitTst(temp, 11)) {
                lcdCommand(L_CLR);
                lcdCommand(L_L1 + 2);
                lcdData('C');
                lcdData('o');
                lcdData('o');
                lcdData('l');
                lcdData(':');
                imprimeLogo(logo3);
                op = 3;
                break;
            }
        }
    }

    if (op == 1) {
        rele();
        potencia();
        tempo(1);
        //cooler    
    }
    if (op == 2) {
        rele();
        potencia();
        tempo(2);
        //aquecimento(2);
        //cooler
    }
    if (op == 3) {
        rele();
        potencia();
        tempo(3);
        //cooler  
    }
    lcdCommand(L_CLR);
    lcdCommand(L_L1 + 4);
    lcdData('A');
    lcdData('p');
    lcdData('e');
    lcdData('r');
    lcdData('t');
    lcdData('e');
    lcdCommand(L_L2 + 6);
    lcdData('R');
    lcdData('S');
    lcdData('T');
    lcdCommand(L_L3 + 5);
    lcdData('P');
    lcdData('a');
    lcdData('r');
    lcdData('a');
    lcdCommand(L_L4 + 3);
    lcdData('R');
    lcdData('e');
    lcdData('i');
    lcdData('n');
    lcdData('i');
    lcdData('c');
    lcdData('i');
    lcdData('a');
    lcdData('r');
    for(;;);
}


