#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Advanced.h"
#include "Image.h"

/* Crop an image */
Image *Crop(Image *image, int x, int y, int W, int H) {
    //First I check if the image pointer exists 
    assert(image);
    // I need to save the old width and height of the image
    int oldWidth = ImageWidth(image);
    int oldHeight = ImageHeight(image);

    // I need to check the bounds of the crop coordinates to make sure they are not larger than the image or negative
    if (x < 0 || y < 0 || x >= oldWidth || y >= oldHeight) {
        printf("Invalid crop coordinates.\n");
        return image;
    }

    // Here I set the width and height of the new image 
    if (x + W > oldWidth) {
        W = oldWidth - x;
    }
    if (y + H > oldHeight) {
        H = oldHeight - y;
    }

    // Next I create a new image based on the cropped width and height
    Image *croppedImage = CreateImage(W, H);
    // Check is the new image exists 
    assert(croppedImage);

    // This loop copies the pixels from the part of the image thats being kept onto the new image
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            SetPixelR(croppedImage, j, i, GetPixelR(image, x + j, y + i));
            SetPixelG(croppedImage, j, i, GetPixelG(image, x + j, y + i));
            SetPixelB(croppedImage, j, i, GetPixelB(image, x + j, y + i));
        }
    }
    // Finally I need to delete the old image and return the new cropped image
    DeleteImage(image);
    return croppedImage;
}

/* Resize an image*/
Image *Resize(Image *image, int newWidth, int newHeight) {
    // I check if the image exists and save its width and height
    assert(image);
    int oldWidth = ImageWidth(image);
    int oldHeight = ImageHeight(image);

    // First I need to create a new image with the provided width and height
    Image *resizedImage = CreateImage(newWidth, newHeight);
    assert(resizedImage);

    // Then I find the scale factors 
    double scaleX = (double)oldWidth / newWidth;
    double scaleY = (double)oldHeight / newHeight;

    // I find the closest neighbor pixel for each pixel of the new image on the old image and map it 
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int srcX = (int)(j * scaleX);
            int srcY = (int)(i * scaleY);
            SetPixelR(resizedImage, j, i, GetPixelR(image, srcX, srcY));
            SetPixelG(resizedImage, j, i, GetPixelG(image, srcX, srcY));
            SetPixelB(resizedImage, j, i, GetPixelB(image, srcX, srcY));
        }
    }
    // At the end I delete the old image and return the resized image
    DeleteImage(image);
    return resizedImage;
}

/* Apply a watermark */
Image *Watermark(Image *image, const Image *watermark_image) {
    // First I check if both the image and the water mark image exist 
    assert(image);
    assert(watermark_image);

    // I need to save the height and width of the old image and the watermark image
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    int waterWidth = ImageWidth(watermark_image);
    int waterHeight = ImageHeight(watermark_image);

    // This loop is to iterate through the height and width of the old image 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // I used % to make sure the watermark wraps around instead of stopping 
            int waterX = x % waterWidth;
            int waterY = y % waterHeight;
            
            // If the watermark image is black at a pixel I need to enhance the brightness
            // of the old image at that pixel by multiplying its RGB color channels by 1.45 at that pixel
            if (GetPixelR(watermark_image, waterX, waterY) == 0 &&
                GetPixelG(watermark_image, waterX, waterY) == 0 &&
                GetPixelB(watermark_image, waterX, waterY) == 0) {
                
                int newR = GetPixelR(image, x, y) * 1.45;
                int newG = GetPixelG(image, x, y) * 1.45;
                int newB = GetPixelB(image, x, y) * 1.45;

                // Finally I set the RGB values at each pixel to the new calculated value and if its over 255
                // I set it to 255. 
                SetPixelR(image, x, y, (newR > 255) ? 255 : newR);
                SetPixelG(image, x, y, (newG > 255) ? 255 : newG);
                SetPixelB(image, x, y, (newB > 255) ? 255 : newB);
            }
        }
    }
    return image;
}

/* Rotate an image by 90 degrees clockwise or counterclockwise */
Image *RotateBy90(Image *image, int rotateDirection) {
    // First I make sure the given image exists 
    assert(image);
    // I also save the width and height of the original image and create a new image 
    // which will be the rotated image created using the old width and height but swapped because
    // a 90 degree rotation would cause them to swap. 
    int oldWidth = ImageWidth(image);
    int oldHeight = ImageHeight(image);
    Image *rotatedImage = CreateImage(oldHeight, oldWidth);
    // Making sure the newly created image exists
    assert(rotatedImage);

    // I loop through every pixel in the old image 
    for (int y = 0; y < oldHeight; y++) {
        for (int x = 0; x < oldWidth; x++) {
            // If the rotation direction is 0 that means its a clockwise rotation so pixel at 
            // x, y gets moved to the location at (oldHeight - 1 - y), x because when rotating clockwise
            // the y coordinate will just be the old x because we are flipping vertically and the
            // new x will be the old height minus y and minus 1 because we start counting from 0. 
            if (rotateDirection == 0) {  
                SetPixelR(rotatedImage, (oldHeight - 1 - y), x, GetPixelR(image, x, y));
                SetPixelG(rotatedImage, (oldHeight - 1 - y), x, GetPixelG(image, x, y));
                SetPixelB(rotatedImage, (oldHeight - 1 - y), x, GetPixelB(image, x, y));
            } 
            // When rotating counter clockwise the pixel at x, y gets moved to the location
            // y, (oldWidth - 1 - x) because the height becomes the x coordinate and the width-x becomes y
            else { 
                SetPixelR(rotatedImage, y, (oldWidth - 1 - x), GetPixelR(image, x, y));
                SetPixelG(rotatedImage, y, (oldWidth - 1 - x), GetPixelG(image, x, y));
                SetPixelB(rotatedImage, y, (oldWidth - 1 - x), GetPixelB(image, x, y));
            }
        }
    }
    // I delete the old image and return the rotated image 
    DeleteImage(image);
    return rotatedImage;
}
