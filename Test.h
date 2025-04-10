#ifndef TEST_H
#define TEST_H

/* Test all DIPs */
int AutoTest(void);

/* test parameters used in AutoTest() */
/* parameters for color filter */
#define CF_TARGET_R 130
#define CF_TARGET_G 130
#define CF_TARGET_B 150
#define CF_THRESHOLD 30
#define CF_REPLACED_R 0
#define CF_REPLACED_G 255
#define CF_REPLACED_B 255

/* border */
#define BORDER_WIDTH 32

/* posterize */
#define R_BITS 7
#define G_BITS 7
#define B_BITS 7

/* motion blur */
#define BLUR_AMOUNT 40

/* pixelate */
#define BLOCK_SIZE 4

/* crop */
#define CROP_X 80
#define CROP_Y 160
#define CROP_WIDTH 250
#define CROP_HEIGHT 100

/* resize */
#define BIG_RESIZE_WIDTH 800
#define BIG_RESIZE_HEIGHT 400
#define SMALL_RESIZE_WIDTH 300
#define SMALL_RESIZE_HEIGHT 200

/* RotateBy90 */
#define ROTATEDIRECTION 0

#endif /* TEST_H */
