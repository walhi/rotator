#ifndef UI_H
#define UI_H

enum workMode {
           port,
           manual,
};

void startupMessage(void);

void printHeader(void);

void printTarget(void);

void printAnt(void);

void printValues(void);

void printUI(void);

#endif
