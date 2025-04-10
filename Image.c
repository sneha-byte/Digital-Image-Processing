#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Image.h"

// This allocates a memory space for the image
Image *CreateImage(unsigned int width, unsigned int height) {
    Image *image = (Image *)malloc(sizeof(Image));
    if (!image) {
        printf("Error allocating memory for image structure.\n");
        return NULL;
    }

    // I need to assign the parameters of the image struct to the correct values so I assign the width and height
    // The given values. 
    image->W = width;
    image->H = height;

    //Here I need to reserve the memory for the pixels for all 3 color channels 
    image->R = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    image->G = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    image->B = (unsigned char *)malloc(width * height * sizeof(unsigned char));

    //Here I check if the memory was successfully allocated and if not I will delete the image 
    if (!image->R || !image->G || !image->B) {
        printf("Error allocating memory for image channels.\n");
        DeleteImage(image);
        return NULL;
    }
    return image;
}

// To prevent memory leaks I will free the dynamically allocated memory from the image structure and color channels.
void DeleteImage(Image *image) {
    if (image) {
        free(image->R);
        free(image->G);
        free(image->B);
        free(image);
    }
}

// These functions are for getting the color intensity of specified channel of pixel (x, y) in the image 
unsigned char GetPixelR(const Image *image, unsigned int x, unsigned int y) {
    //Check to make sure the image and the color channel R of the image exist 
    assert(image && image->R);
    return image->R[y * image->W + x];
}

unsigned char GetPixelG(const Image *image, unsigned int x, unsigned int y) {
    assert(image && image->G);
    return image->G[y * image->W + x];
}

unsigned char GetPixelB(const Image *image, unsigned int x, unsigned int y) {
    assert(image && image->B);
    return image->B[y * image->W + x];
}

// These functions are for setting the color intensity of specified channel of pixel (x, y) in the image 
void SetPixelR(Image *image, unsigned int x, unsigned int y, unsigned char r) {
    assert(image && image->R);
    image->R[y * image->W + x] = r;
}

// This checks if image and color channel G exists and then sets the G color channel at that pixel to g
void SetPixelG(Image *image, unsigned int x, unsigned int y, unsigned char g) {
    assert(image && image->G);
    image->G[y * image->W + x] = g;
}

void SetPixelB(Image *image, unsigned int x, unsigned int y, unsigned char b) {
    assert(image && image->B);
    image->B[y * image->W + x] = b;
}

// These functions will return the image width and height in pixels
unsigned int ImageWidth(const Image *image) {
    return image->W;
}

unsigned int ImageHeight(const Image *image) {
    return image->H;
}
