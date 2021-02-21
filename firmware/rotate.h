#ifndef ROTATE_H
#define ROTATE_H

struct dir {
  unsigned right: 1;
  unsigned right_overlap: 1;
  unsigned left: 1;
  unsigned left_overlap: 1;
  unsigned up: 1;
  unsigned down: 1;
  unsigned wire_right;
  unsigned wire_left;
};

void rotateInit(void);

void azimuthImpulse(void);

void elevationImpulse(void);

#endif
