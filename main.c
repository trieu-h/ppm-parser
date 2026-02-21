#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <raylib.h>

typedef struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;

#define WINDOW_SIZE 400
#define PIXEL_SIZE 100

int main()
{
  FILE *fp;
  fp = fopen("sample.ppm", "r");
  if (fp == NULL) {
    printf("Error: %s\n", strerror(errno));
  }

  char magic_number[3];
  fscanf(fp, "%s", magic_number);

  if (strcmp(magic_number, "P3") != 0) {
    printf("Error: Invalid file format\n");
    exit(1);
  }

  size_t w, h;
  if (fscanf(fp, "%zu %zu", &w, &h) < 2) {
    printf("Error: Could not find width and height\n");
    exit(1);
  } else {
    printf("width: %zu, height: %zu\n", w, h);
  }

  uint16_t maxval;
  if (fscanf(fp, "%hu", &maxval) < 1) {
    printf("Error: Could not find maxval\n");
    exit(1);
  } else {
    printf("maxval: %hu\n", maxval);
  }

  Pixel pxs[h][w];

  printf("pixels: \n");
  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      uint8_t r, g, b;
      if (fscanf(fp, "%" SCNu8 " %" SCNu8 " %" SCNu8, &r, &g, &b) < 3) {
        printf("Error: Could not find r, g, b values\n");
        exit(1);
      } else {
        printf("%" SCNu8 " %" SCNu8 " %" SCNu8 " ", r, g, b);
      }
      pxs[y][x] = (Pixel) {r, g, b};
    }
    printf("\n");
  }

  fclose(fp);

  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "PPM");
  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
          Pixel p = pxs[y][x];
          DrawRectangle(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, (Color) {p.r, p.g, p.b, 255});
        }
      }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

