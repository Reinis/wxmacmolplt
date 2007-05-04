#ifndef __AtomPatterns__
#define __AtomPatterns__

#ifdef __WXMAC__
#include <AGL/glu.h>
#else
#include <GL/glu.h>
#endif

const int numPatterns = 19;

const GLubyte atomMaskPatterns[numPatterns][128] =
  {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //pattern 0

  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, //pattern 1

  {0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
    0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00,
    0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18,
   0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x01}, //pattern 2

  {0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c,
   0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00,
   0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00,
   0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
   0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
   0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00},  //pattern 3

  {0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18, 0x00,
    0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x80, 0x01, 0x80,
    0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18,
    0x00, 0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x80, 0x01, 0x80, 0x01,
    0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18, 0x00,
    0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x80, 0x01, 0x80,
    0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18,
    0x00, 0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x80, 0x01, 0x80, 0x01}, //pattern 4

  {0x80, 0x01, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c,
   0x00, 0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0,
   0x01, 0x80, 0x01, 0x80, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c, 0x00,
   0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0, 0x00,
   0x80, 0x01, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c,
   0x00, 0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0,
   0x01, 0x80, 0x01, 0x80, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c, 0x00,
   0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0, 0x00}, //pattern 5

  {0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01},  //pattern 6

  {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   },  //pattern 7

  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   },  //pattern 8 "brick"

  {0xf0, 0x0f, 0xe0, 0x0f, 0xe0, 0x07, 0xc0, 0x07, 0xc0, 0x03, 0x80, 0x03, 0x80, 0x01, 0x00, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x01, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x03, 0xe0, 0x07, 0xc0, 0x07, 0xf0, 0x0f, 0xe0, 0x0f,
   0xe0, 0x07, 0xc0, 0x07, 0xc0, 0x03, 0x80, 0x03, 0x80, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x01, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x03, 0xe0, 0x07, 0xc0, 0x07, 0xf0, 0x0f, 0xe0, 0x0f
   },  //pattern 9

  {0xe0, 0x00, 0x38, 0x00, 0xe7, 0x00, 0x39, 0xc0, 0xe7, 0x00, 0x39, 0xc0, 0x07, 0x00, 0x01, 0xc0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x0e, 0x1c, 0x1c, 0x00, 0x0e,
   0x1c, 0x00, 0xe0, 0x00, 0x1c, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x1c,
   0xe0, 0x00, 0x38, 0x00, 0xe7, 0x00, 0x39, 0xc0, 0xe7, 0x00, 0x39, 0xc0, 0x07, 0x00, 0x01, 0xc0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x0e, 0x1c, 0x1c, 0x00, 0x0e,
   0x1c, 0x00, 0xe0, 0x00, 0x1c, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x1c
   },  //pattern 10

  {0xc0, 0x18, 0xc0, 0x18, 0x60, 0x30, 0x60, 0x30, 0x30, 0x60, 0x30, 0x60, 0x18, 0xc0, 0x18, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80,
    0x18, 0xc0, 0x18, 0xc0, 0x30, 0x60, 0x30, 0x60, 0x60, 0x30, 0x60, 0x30, 0xc0, 0x18, 0xc0, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
    0xc0, 0x18, 0xc0, 0x18, 0x60, 0x30, 0x60, 0x30, 0x30, 0x60, 0x30, 0x60, 0x18, 0xc0, 0x18, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0xc0, 0x18, 0xc0, 0x30, 0x60, 0x30, 0x60, 0x60, 0x30, 0x60, 0x30, 0xc0, 0x18, 0xc0, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00}, //pattern 11

  {0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00,
   0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38,
    0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07,
    0xe0, 0x18, 0xc0, 0x38, 0xe0, 0x18, 0xc0, 0x38, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7,
   0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00,
   0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38,
    0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07,
    0xe0, 0x18, 0xc0, 0x38, 0xe0, 0x18, 0xc0, 0x38, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7
    }, //pattern 12

  {0x80, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x00, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x00, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 13

  {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
   },  //pattern 14

  {0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 15

  {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 16

  {0x7f, 0xfc, 0xff, 0xfe, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0xcf, 0xfc, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xfe, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x7f, 0xfc, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00
   },  //pattern 17

  {0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
   }  //pattern 18
  };

#endif
