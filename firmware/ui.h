#ifndef UI_H
#define UI_H

enum workMode {
               none,
               port,
               manual,
};

void startupMessage(void);

void initUI(void);

void printTarget(void);

void printAnt(void);

void printValues(void);

void printUI(void);

void printBigAnt(int16_t value);
#endif
