# include <stdio.h>
# include "DIPs.h"
# include "Image.h"
# include <assert.h>

/* Black and White */
Image *BlackNWhite(Image *image) {
    // First I check that the inputted image pointer exists 
    assert(image);
    // I get the height and width of this image using the functions in Image.c 
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // Then I loop through the height and width to find the average of the color channels at each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int gray = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
            //Then I set all the colors channels of each pixel to the new value I calculate above using the 
            // SetPixel function in Image.c
            SetPixelR(image, x, y, gray);
            SetPixelG(image, x, y, gray);
            SetPixelB(image, x, y, gray);
        }
    }
    return image;
}

/* Reverse image color */
Image *Negative(Image *image) {
    // This makes sure my inputted image pointer exists  
    assert(image);
    //Here I get the width and height of the image and assign it to width and height local variables 
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // I loop through the height and width and assign all pixels their new values using given formula 
    // for negative which is 255 - current value 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SetPixelR(image, x, y, MAX_PIXEL - GetPixelR(image, x, y));
            SetPixelG(image, x, y, MAX_PIXEL - GetPixelG(image, x, y));
            SetPixelB(image, x, y, MAX_PIXEL - GetPixelB(image, x, y));
        }
    }
    return image;
}

/* Color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {
    // First I check that the inputted image pointer exists and is valid 
    assert(image); 
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // I find out the replacement values the same way that I did in assignment 2 by checking the threshold value
    // and using the given method
    replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
    replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
    replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

    // I loop through all the pixels and get the value of each color channel at each pixel using the GetPixel 
    // functions in create image and then if value is in range I set that pixels color channels to the replacement values
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r = GetPixelR(image, x, y);
            int g = GetPixelG(image, x, y);
            int b = GetPixelB(image, x, y);

            // Finally, I check for overflow or out of bounds before I set the new values to the color channels of the pixel
            if (abs(r - target_r) <= threshold && abs(g - target_g) <= threshold && abs(b - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
            }
        }
    }
    return image;
}

/* Edge detection */
Image *Edge(Image *image) {
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // I create a temporary image to store the old pixel values so they dont get overwritten 
    Image *temp = CreateImage(width, height);
    assert(temp);

    // Loop through the old image and set the temp images pixels to the same as the old one 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SetPixelR(temp, x, y, GetPixelR(image, x, y));
            SetPixelG(temp, x, y, GetPixelG(image, x, y));
            SetPixelB(temp, x, y, GetPixelB(image, x, y));
        }
    }

    // Here I compute the edges using the same method as before by iterating over each pixel (x, y), 
    // and comparing the RGB values of it with its neighbor pixels and summing the differences between 
    // the pixel and each of its neighbors
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // I sum the differences in the color channels separately 
            int sumR = 0, sumG = 0, sumB = 0;

            for (int n = -1; n <= 1; n++) {
                for (int m = -1; m <= 1; m++) {
                    int a = x + m;
                    int b = y + n;
                    sumR += (GetPixelR(temp, x, y) - GetPixelR(temp, a, b));
                    sumG += (GetPixelG(temp, x, y) - GetPixelG(temp, a, b));
                    sumB += (GetPixelB(temp, x, y) - GetPixelB(temp, a, b));
                }
            }

            // I check the values to make sure nothing is out of bounds and then I set the new values using
            // SetPixel function. If the sum is over 255 I make it 255 and if its below 0 I make it 0
            SetPixelR(image, x, y, (sumR > MAX_PIXEL) ? MAX_PIXEL : (sumR < 0) ? 0 : sumR);
            SetPixelG(image, x, y, (sumG > MAX_PIXEL) ? MAX_PIXEL : (sumG < 0) ? 0 : sumG);
            SetPixelB(image, x, y, (sumB > MAX_PIXEL) ? MAX_PIXEL : (sumB < 0) ? 0 : sumB);
        }
    }
    // I am setting the border pixels to zero here as described in the assignment 
    for (int x = 0; x < width; x++) {  
        SetPixelR(image, x, 0, 0);
        SetPixelG(image, x, 0, 0);
        SetPixelB(image, x, 0, 0);
        
        SetPixelR(image, x, height - 1, 0);
        SetPixelG(image, x, height - 1, 0);
        SetPixelB(image, x, height - 1, 0);
    }

    for (int y = 0; y < height; y++) {  
        SetPixelR(image, 0, y, 0);
        SetPixelG(image, 0, y, 0);
        SetPixelB(image, 0, y, 0);
        
        SetPixelR(image, width - 1, y, 0);
        SetPixelG(image, width - 1, y, 0);
        SetPixelB(image, width - 1, y, 0);
    }

    // I delete the temp image and return the new image
    DeleteImage(temp);
    return image;
}

/* Flip image vertically */
Image *VFlip(Image *image) {
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    
    // I am going up to half the height and swapping the top half with the bottom half using the same algorithm
    // I used before. I iterate over each pixel (x, y), and get the RGB values from the corresponding pixel 
    // which is at (x, height - 1 - y) and swap the 2 of them 
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = GetPixelR(image, x, height - 1 - y);
            unsigned char g = GetPixelG(image, x, height - 1 - y);
            unsigned char b = GetPixelB(image, x, height - 1 - y);

            // Here I am getting the color from the original x, y and setting that color to the x, height-1-y
            SetPixelR(image, x, height - 1 - y, GetPixelR(image, x, y));
            SetPixelG(image, x, height - 1 - y, GetPixelG(image, x, y));
            SetPixelB(image, x, height - 1 - y, GetPixelB(image, x, y));

            // Here I am setting the RGB of the top half to the ones from the bottom half 
            SetPixelR(image, x, y, r);
            SetPixelG(image, x, y, g);
            SetPixelB(image, x, y, b);
        }
    }
    return image;
}

/* Mirror image horizontally */
Image *HMirror(Image *image) {
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // I use the same method as the last assignment to mirror the pixels on the left onto the pixels on the right
    // and then I use the SetPixel functions to get the pixel on the left and set it to the pixel on the right
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            int mirrorX = width - 1 - x;
            SetPixelR(image, mirrorX, y, GetPixelR(image, x, y));
            SetPixelG(image, mirrorX, y, GetPixelG(image, x, y));
            SetPixelB(image, mirrorX, y, GetPixelB(image, x, y));
        }
    }
    return image;
}

/* Add border to an image */
Image *AddBorder(Image *image, char *color, int border_width) {
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // By default all the boarder values are set to 255
    int border_r = 255, border_g = 255, border_b = 255;

    // Here I find the RGB values for each border color that can be inputted
    if (strcmp(color, "black") == 0) { 
        border_r = 0;
        border_g = 0; 
        border_b = 0; 
    }
    else if (strcmp(color, "red") == 0) { 
        border_g = 0;
        border_b = 0; 
    }
    else if (strcmp(color, "green") == 0) { 
        border_r = 0;
        border_b = 0; 
    }
    else if (strcmp(color, "blue") == 0) { 
        border_r = 0;
        border_g = 0; 
    }
    else if (strcmp(color, "yellow") == 0) { 
        border_b = 0; 
    }
    else if (strcmp(color, "cyan") == 0) { 
        border_r = 0; 
    }
    else if (strcmp(color, "pink") == 0) { 
        border_r = 255; 
        border_g = 192; 
        border_b = 203; 
    }
    else if (strcmp(color, "orange") == 0) { 
        border_r = 255; 
        border_g = 165; 
        border_b = 0; 
    }
    else {
        printf("Unsupported color.\n");
        return image;
    }

    // I figure out where the boarder lies using the same method as the previous assignment and then I set the color channels of 
    // the pixels in that range to the specified border values. 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y < border_width || y >= height - border_width || x < border_width || x >= width - border_width) {
                SetPixelR(image, x, y, border_r);
                SetPixelG(image, x, y, border_g);
                SetPixelB(image, x, y, border_b);
            }
        }
    }
    return image;
}

/* Shuffle an image */
Image *Shuffle(Image *image) {
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    //I need to first calculate the height and width of the blocks 
    int block_width = width / SHUFF_WIDTH_DIV;
    int block_height = height / SHUFF_HEIGHT_DIV;
    
    // I use a temporary image to swap the blocks without overriding anything 
    Image *temp = CreateImage(block_width, block_height);
    assert(temp);

    // I iterate through half of the blocks to swap block 0 with block 15, 1 with 14, ect. 
    for (int i = 0; i < SHUFF_HEIGHT_DIV / 2; i++) {
        for (int j = 0; j < SHUFF_WIDTH_DIV; j++) {
            for (int x = 0; x < block_width; x++) {
                for (int y = 0; y < block_height; y++) {
                    int src_x = x + (j * block_width);
                    int src_y = y + (i * block_height);
                    int dest_x = x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width);
                    int dest_y = y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height);

                    // These are for storing the original pixel in the temporary image 
                    SetPixelR(temp, x, y, GetPixelR(image, src_x, src_y));
                    SetPixelG(temp, x, y, GetPixelG(image, src_x, src_y));
                    SetPixelB(temp, x, y, GetPixelB(image, src_x, src_y));

                    // These are to set the swapped pixels to the image
                    SetPixelR(image, src_x, src_y, GetPixelR(image, dest_x, dest_y));
                    SetPixelG(image, src_x, src_y, GetPixelG(image, dest_x, dest_y));
                    SetPixelB(image, src_x, src_y, GetPixelB(image, dest_x, dest_y));

                    // This is to set the returning image pixels based on the previous calculations
                    SetPixelR(image, dest_x, dest_y, GetPixelR(temp, x, y));
                    SetPixelG(image, dest_x, dest_y, GetPixelG(temp, x, y));
                    SetPixelB(image, dest_x, dest_y, GetPixelB(temp, x, y));
                }
            }
        }
    }

    //Delete the temporary image before returning the original modified image
    DeleteImage(temp);
    return image;
}

/* Pixelate an image */
Image *Pixelate(Image *image, int block_size) {
    // I first make sure the image exists 
    assert(image);
    // I save the image width and height 
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // I iterate by block size instead of by pixel because the pixelate is done by blocks. 
    // This outerloop is for iterating over the blocks
    for (int y = 0; y < height; y += block_size) {
        for (int x = 0; x < width; x += block_size) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            int count = 0;

            // I use the innerloop to iterate Within the block and the x and y position keep track
            // of the coordinate in the block I am on
            for (int j = 0; j < block_size; j++) {
                for (int i = 0; i < block_size; i++) {
                    int xPosition = x + i;
                    int yPostion = y + j;

                    // I am supposed to ignore boundary pixels if they are not a full block
                    // so this is meant to prevent errors about out of bounds
                    if (xPosition >= width || yPostion >= height) {
                        continue;
                    }

                    // Here I am summing the RGB values of all the pixels inside the single block
                    // So I can use the sum to find the average RGB value 
                    sumR += GetPixelR(image, xPosition, yPostion);
                    sumG += GetPixelG(image, xPosition, yPostion);
                    sumB += GetPixelB(image, xPosition, yPostion);
                    count++;
                }
            }

            // This is to make sure I dont get an error from dividing by zero
            if (count == 0) {
                continue; 
            }

            // I calculate the average of the RGB values here that I summed previously
            int averageR = sumR / count;
            int averageG = sumG / count;
            int averageB = sumB / count;

            // This loop is to make the entire block the average color using SetPixel functions
            for (int j = 0; j < block_size; j++) {
                for (int i = 0; i < block_size; i++) {
                    int xPosition = x + i;
                    int yPosition = y + j;

                    // Here I am checking to avoid out of bounds error 
                    if (xPosition >= width || yPosition >= height) {
                        continue;
                    }

                    // I am setting the pixels in the block to the average color here because 
                    // the outerloop is iterating through the pixels
                    SetPixelR(image, xPosition, yPosition, averageR);
                    SetPixelG(image, xPosition, yPosition, averageG);
                    SetPixelB(image, xPosition, yPosition, averageB);
                }
            }
        }
    }
    return image;
}

/* Shift an image */
Image *Shift(Image *image, int shiftX, int shiftY) {
    // Checking if image exists and saving width and height
    assert(image);
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // Here I am creating an image of the same width and height to avoid overwriting my main image
    Image *shiftedImage = CreateImage(width, height);
    // I verify it exists 
    assert(shiftedImage);

    // In this loop I iterate over every pixel in the image 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // I am shifting the pixel at x, y to newX, newY by calculating newX and newY using the equation
            // below. The % operator makes sure the image wraps around. 
            int newX = (x + shiftX) % width;
            int newY = (y + shiftY) % height;
            // This part checks if newX or newY end up negative then I would add width or height to the 
            // negative value to wrap it 
            if (newX < 0) {
                newX += width;
            }
            if (newY < 0) {
                newY += height;
            }

            // Here I am assigning the new pixels their new values. By using the GetPixelR
            // I get the value of the color channel from the original image at x, y, and 
            // SetPixel places it at (newX, newY) of shifted image 
            SetPixelR(shiftedImage, newX, newY, GetPixelR(image, x, y));
            SetPixelG(shiftedImage, newX, newY, GetPixelG(image, x, y));
            SetPixelB(shiftedImage, newX, newY, GetPixelB(image, x, y));
        }
    }
    // Finally I delete the old image and return the shifted image 
    DeleteImage(image);
    return shiftedImage;
}


