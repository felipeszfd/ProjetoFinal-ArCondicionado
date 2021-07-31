#include <xc.h>
#include <pic18f4520.h>
#include "funcoes.h"
#include "lcd.h"
#include "config.h"
#include "keypad.h"
#include "lcd.h"
#include "bits.h"
#include "ssd.h"
#include "atraso.h"
#include "adc.h"
#include "itoa.h"
#include "pwm.h"
#include "io.h"

#define L_L1 0x80
#define L_L2 0xC0
#define L_L3 0x90
#define L_L4 0xD0
#define L_CLR 0x01
#define ON 0x0F

unsigned int tmpi;
char str[6];
unsigned char cnt;
int oppot = 0;

void ConfiguraLed() {
    TRISD = 0x00;
    PORTD = 0x00;
}

void LigarLed(char num) {
    unsigned char aux = PORTD;
    if (num == 0) {
        PORTD = aux | 0x01;
    }
    if (num == 1) {
        PORTD = aux | 0x02;
    }
    if (num == 2) {
        PORTD = aux | 0x04;
    }
    if (num == 3) {
        PORTD = aux | 0x08;
    }
    if (num == 4) {
        PORTD = aux | 0x10;
    }
    if (num == 5) {
        PORTD = aux | 0x20;
    }
    if (num == 6) {
        PORTD = aux | 0x40;
    }
    if (num == 7) {
        PORTD = aux | 0x80;
    }
}

void DesligarLed(char num) {
    unsigned char aux = PORTD;
    if (num == 0) {
        PORTD = aux ^ 0x01;
    }
    if (num == 1) {
        PORTD = aux ^ 0x02;
    }
    if (num == 2) {
        PORTD = aux ^ 0x04;
    }
    if (num == 3) {
        PORTD = aux ^ 0x08;
    }
    if (num == 4) {
        PORTD = aux ^ 0x10;
    }
    if (num == 5) {
        PORTD = aux ^ 0x20;
    }
    if (num == 6) {
        PORTD = aux ^ 0x40;
    }
    if (num == 7) {
        PORTD = aux ^ 0x80;
    }
}

void imprimeLogo(char op[32]) {
    char i = 0;
    lcdCommand(0x40);
    for (i = 0; i < 32; i++) {
        lcdData(op[i]);
    }
    lcdCommand(L_L2 + 7);
    lcdData(0);
    lcdData(1);
    lcdCommand(L_L3 + 7);
    lcdData(2);
    lcdData(3);
    atraso_ms(1000);
}

void temperatura() {
    unsigned char i;

    TRISA = 0x07;

    adc_init();


    for (i = 0; i < 100; i++) {
        tmpi = (adc_amostra(1)*10) / 2;
        lcdCommand(L_L3 + 5);
        itoa(tmpi, str);
        lcdData(str[2]);
        lcdData(str[3]);
        lcdData(',');
        lcdData(str[4]);
        lcdData('C');
        atraso_ms(20);
    }
}

void desligaDisplay(){
    digitalWrite(PIN_A2, LOW);
    digitalWrite(PIN_A3, LOW);
    digitalWrite(PIN_A4, LOW);
    digitalWrite(PIN_A5, LOW);
}

void menuTempo(unsigned long int val) {
    volatile int tempo;
    unsigned long int i;
    unsigned long int cont = val;
    ssdInit();
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 5);
    lcdData('L');
    lcdData('I');
    lcdData('G');
    lcdData('A');
    lcdData('D');
    lcdData('O');
    //temperatura();
    for (i = val; i > 0; i--) {
        cont--;
        ssdDigit(((cont / 60000) % 10), 0);
        ssdDigit(((cont / 6000) % 10), 1); //60s
        ssdDigit(((cont / 1000) % 6), 2); //10s
        ssdDigit(((cont / 100) % 10), 3); //1s
        ssdUpdate();
        for (tempo = 0; tempo < 575; tempo++);
    }
    desligaDisplay();
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 3);
    lcdData('E');
    lcdData('N');
    lcdData('C');
    lcdData('E');
    lcdData('R');
    lcdData('R');
    lcdData('A');
    lcdData('D');
    lcdData('O');
    atraso_ms(2000);
}

void aquecimento(unsigned int val) {
    unsigned char i;
    pwmInit();
    TRISC = 0x01;
    PORTC = 0x00;
    lcdCommand(L_CLR);
    bitSet(PORTC, 5);
    if (oppot == 1) {
        pwmSet1(98);
    }
    if (oppot == 2) {
        pwmSet1(90);
    }
    if (oppot == 3) {
        pwmSet1(80);
    }
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 6);
    lcdData('T');
    lcdData('I');
    lcdData(':');
    temperatura();
    menuTempo(val);
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 6);
    lcdData('T');
    lcdData('F');
    lcdData(':');
    temperatura();
    bitClr(PORTC, 5);
    pwmSet1(0);
}

void resfriamento(unsigned int val) {
    unsigned char i;
    pwmInit();
    TRISC = 0x01;
    PORTC = 0x00;
    TRISA = 0x07;
    lcdCommand(L_CLR);
    if (oppot == 1) {
        pwmSet1(32);
    }
    if (oppot == 2) {
        pwmSet1(64);
    }
    if (oppot == 3) {
        pwmSet1(96);
    }
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 6);
    lcdData('T');
    lcdData('I');
    lcdData(':');
    temperatura();
    menuTempo(val);
    lcdCommand(L_CLR);
    lcdCommand(L_L2 + 6);
    lcdData('T');
    lcdData('F');
    lcdData(':');
    lcdCommand(L_L3 + 5);
    lcdData('2');
    lcdData('5');
    lcdData(',');
    lcdData('0');
    lcdData('C');
    for (i = 0; i < 100; i++) 
        atraso_ms(20);
    pwmSet1(0);
}

void potencia() {
    unsigned int tmp;
    unsigned int tecla = 16;
    lcdCommand(L_CLR);
    lcdCommand(L_L1 + 4);
    lcdData('E');
    lcdData('s');
    lcdData('c');
    lcdData('o');
    lcdData('l');
    lcdData('h');
    lcdData('a');
    lcdCommand(L_L2 + 2);
    lcdData('a');
    lcdData(' ');
    lcdData('P');
    lcdData('o');
    lcdData('t');
    lcdData('e');
    lcdData('n');
    lcdData('c');
    lcdData('i');
    lcdData('a');
    lcdData(':');
    lcdCommand(L_L4);
    lcdData('(');
    lcdData('*');
    lcdData(')');
    lcdData('C');
    lcdData('o');
    lcdData('n');
    lcdData('f');
    lcdData('i');
    lcdData('r');
    lcdData('m');
    lcdData('a');
    for (;;) {
        tmp = (adc_amostra(0)*10) / 204;
        lcdCommand(L_L3 + 7);
        itoa(tmp, str);
        if (tmp < 16) {
            lcdData('L');
            oppot = 1;
        }
        if (tmp >= 16 && tmp < 32) {
            lcdData('M');
            oppot = 2;
        }
        if (tmp >= 32) {
            lcdData('H');
            oppot = 3;
        }
        atraso_ms(10);
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 0)) {
                break;
            }
        }
    }
}

void tempo(int op) {
    unsigned long int tempo;
    unsigned int tecla = 16;
    unsigned long int val = 0, i;
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdData('T');
    lcdData('e');
    lcdData('m');
    lcdData('p');
    lcdData('o');
    lcdData(':');
    lcdCommand(L_L2);
    lcdData(' ');
    lcdData('4');
    lcdData('-');
    lcdData('2');
    lcdData(':');
    lcdData('0');
    lcdData('0');
    lcdCommand(L_L3);
    lcdData(' ');
    lcdData('5');
    lcdData('-');
    lcdData('5');
    lcdData(':');
    lcdData('0');
    lcdData('0');
    lcdCommand(L_L4);
    lcdData(' ');
    lcdData('6');
    lcdData('-');
    lcdData('1');
    lcdData('0');
    lcdData(':');
    lcdData('0');
    lcdData('0');
    ssdInit();
    for (;;) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 2)) {
                val = 12000;
                if (op == 1) {
                    menuTempo(val);
                    break;
                }
                if (op == 2) {
                    aquecimento(val);
                    break;
                }
                if (op == 3) {
                    resfriamento(val);
                    break;
                }
            }
            if (bitTst(tecla, 6)) {
                val = 30000;
                if (op == 1) {
                    menuTempo(val);
                    break;
                }
                if (op == 2) {
                    aquecimento(val);
                    break;
                }
                if (op == 3) {
                    resfriamento(val);
                    break;
                }
            }
            if (bitTst(tecla, 10)) {
                val = 60000;
                if (op == 1) {
                    menuTempo(val);
                    break;
                }
                if (op == 2) {
                    aquecimento(val);
                    break;
                }
                if (op == 3) {
                    resfriamento(val);
                    break;
                }
            }
        }
    }
}

void rele() {
    unsigned char i;
    TRISCbits.TRISC0 = 0;
    TRISEbits.TRISE0 = 0;
    for (i = 0; i < 5; i++) {
        PORTCbits.RC0 ^= 1;
        atraso_ms(500);
    }
    PORTCbits.RC0 = 0;
    for (i = 0; i < 5; i++) {
        PORTEbits.RE0 ^= 1;
        atraso_ms(500);
    }
    PORTEbits.RE0 = 0;
}


