#ifndef UI_H
#define UI_H

enum workMode {
	WORK_NONE,
	WORK_PORT,
	WORK_MANUAL,
};

void startupMessage(void);

void initUI(void);

void printUI(void);

#endif
