#ifndef PLATFORM_H
#define PLATFORM_H

/* SDCC для AT89S52 сам подставляет чтение   */
/* из области FLASH                          */
#define CODE_MEM_BEFORE __code
#define CODE_MEM_AFTER
#define memcpy_P(x, y, z) (void)1


#endif
