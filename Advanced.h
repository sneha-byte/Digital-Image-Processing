#ifndef ADVANCED_H_INCLUDED_
#define ADVANCED_H_INCLUDED_

#include "Constants.h"
#include "Image.h"

/* Crop */
Image *Crop(Image *image, int x, int y, int W, int H);

/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight);

/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image);

/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection);
#endif /* ADVANCED_H_INCLUDED_ */
