#include "draw.h"
#include <stdbool.h>

LCDBitmap *lcdBitmap = NULL;
PlaydateAPI *pd = NULL;
int disp_width = -1;
int disp_height = -1;

enum eColor {
  black = false,
  white = true
};

typedef enum eColor Color;

void init(void) {
  disp_width = pd->display->getWidth();
  disp_height = pd->display->getHeight();
  pd->system->logToConsole("width: %d, height: %d", disp_width, disp_height);
  lcdBitmap = pd->graphics->newBitmap(disp_width, disp_height, kColorBlack);
}

void changePixel(int x, int y, Color v) {
  int width, height, rowbytes;
  uint8_t *bitmap_mask, *data;
  pd->graphics->getBitmapData(lcdBitmap, &width, &height, &rowbytes, &bitmap_mask, &data);

  int byte_count = (width * height) / 8; // 1 bit per pixel

  int xbyte = x / 8;
  int ybyte = y * rowbytes;

  int byte = xbyte + ybyte;
  int bit = x - (xbyte * 8);

  int mask = 128 >> bit;

  //pd->system->logToConsole("(%d, %d) = %d.%d  %d", x, y, byte, bit, mask);

  if (v) {
    data[byte] |= mask;
  }
  if (!v) {
    data[byte] &= ~mask;
  }

  //pd->system->logToConsole("%d", data[byte]);
}

void setPixel(int x, int y) {
  changePixel(x, y, black);
}

void clearPixel(int x, int y) {
  changePixel(x, y, white);
}

void draw(PlaydateAPI *pdapi) {
  static bool didInit = false;
  static int x = 0, y = 0;

  if (pd == NULL) {
    pd = pdapi;
  }
  if (!didInit) {
    init();
    didInit = true;
  }

  changePixel(x, y, white);

  x = (x + 1) % disp_width;
  if (x==0) {
    y = (y + 1) % disp_height;
  }

  pd->graphics->drawBitmap(lcdBitmap, 0, 0, kBitmapUnflipped);
}
