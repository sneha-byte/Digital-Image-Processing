#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Image.h"
#include "Advanced.h"
#include "DIPs.h"
#include "FileIO.h"
#include "Test.h"

void PrintMenu(void);

int main() {
    Image *image = NULL;
    int option;
    char fname[SLEN];
    char colorOption[SLEN];

    // Here I am declaring the variables I need outside the switch cases 
    int target_r, target_g, target_b, threshold;
    int replace_r, replace_g, replace_b;
    int border_width;
    int factor;
    int shiftX, shiftY;
    int x, y, cropW, cropH;
    int newWidth, newHeight;
    int direction;
    int testResult;

// If Debug mode is on, I will only run AutoTest without printing the menu 
#ifdef DEBUG
    image = LoadImage("EngPlaza");
    AutoTest();
    DeleteImage(image);
    return 0;
#else
    PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);

    // I check if the exit option is clicked and if not I go into the other options
    while (option != 18) {  
        if (option == 1) {  
            printf("Please input the file name to load: ");
            scanf("%75s", fname);
            if (image) {
                DeleteImage(image);
            }
            image = LoadImage(fname);
            if (!image) {
                printf("Error: Unable to load image.\n");
            }
        } 
        else if (option >= 2 && option <= 17) {
            if (!image) {
                printf("No image to process!\n");
            } else {
                // These are switch cases for the options besides load image 
                switch (option) {
                    case 2:
                        printf("Please input the file name to save: ");
                        scanf("%75s", fname);
                        SaveImage(fname, image);
                        break;

                    case 3:
                        // Case 3 is black and white so I put the image object into the black and white function
                        image = BlackNWhite(image);
                        printf("\"Black & White\" operation is done!\n");
                        break;

                    case 4:
                        image = Negative(image);
                        printf("\"Negative\" operation is done!\n");
                        break;

                    case 5:
                        printf("Enter Red component for the target color: ");
                        scanf("%d", &target_r);
                        printf("Enter Green component for the target color: ");
                        scanf("%d", &target_g);
                        printf("Enter Blue component for the target color: ");
                        scanf("%d", &target_b);
                        printf("Enter threshold for the color difference: ");
                        scanf("%d", &threshold);
                        printf("Enter replacement Red component: ");
                        scanf("%d", &replace_r);
                        printf("Enter replacement Green component: ");
                        scanf("%d", &replace_g);
                        printf("Enter replacement Blue component: ");
                        scanf("%d", &replace_b);
                        image = ColorFilter(image, target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b);
                        printf("\"Color Filter\" operation is done!\n");
                        break;

                    case 6:
                        image = Edge(image);
                        printf("\"Edge\" operation is done!\n");
                        break;

                    case 7:
                        image = Shuffle(image);
                        printf("\"Shuffle\" operation is done!\n");
                        break;

                    case 8:
                        image = VFlip(image);
                        printf("\"VFlip\" operation is done!\n");
                        break;

                    case 9:
                        image = HMirror(image);
                        printf("\"HMirror\" operation is done!\n");
                        break;

                    case 10:
                        printf("Enter border width: ");
                        scanf("%d", &border_width);
                        printf("Available border colors: black, white, red, green, blue, yellow, cyan, pink, orange\n");
                        printf("Select border color: ");
                        scanf("%s", colorOption);
                        image = AddBorder(image, colorOption, border_width);
                        printf("\"Border\" operation is done!\n");
                        break;

                    case 11:
                        printf("Enter the pixelation factor: ");
                        scanf("%d", &factor);
                        image = Pixelate(image, factor);
                        printf("\"Pixelate\" operation is done!\n");
                        break;

                    case 12:
                        printf("Enter horizontal shift value: ");
                        scanf("%d", &shiftX);
                        printf("Enter vertical shift value: ");
                        scanf("%d", &shiftY);
                        image = Shift(image, shiftX, shiftY);
                        printf("\"Shift\" operation is done!\n");
                        break;

                    case 13:
                        printf("Enter the X offset for cropping: ");
                        scanf("%d", &x);
                        printf("Enter the Y offset for cropping: ");
                        scanf("%d", &y);
                        printf("Enter the crop width: ");
                        scanf("%d", &cropW);
                        printf("Enter the crop height: ");
                        scanf("%d", &cropH);
                        image = Crop(image, x, y, cropW, cropH);
                        printf("\"Crop\" operation is done!\n");
                        break;

                    case 14:
                        printf("Enter new image width: ");
                        scanf("%d", &newWidth);
                        printf("Enter new image height: ");
                        scanf("%d", &newHeight);
                        image = Resize(image, newWidth, newHeight);
                        printf("\"Resize\" operation is done!\n");
                        break;

                    case 15:
                        printf("Applying Watermark...\n");
                        Image *watermark = LoadImage("watermark_template");
                        if (watermark) {
                            image = Watermark(image, watermark);
                            DeleteImage(watermark);
                            printf("\"Watermark\" operation is done!\n");
                        } else {
                            printf("Error: Could not load watermark image.\n");
                        }
                        break;

                    case 16:
                        printf("Enter the direction (0: clockwise, 1: counterclockwise): ");
                        scanf("%d", &direction);
                        image = RotateBy90(image, direction);
                        printf("\"RotateBy90\" operation is done!\n");
                        break;

                    case 17:
                        testResult = AutoTest();
                        if (testResult != 0) {
                            printf("AutoTest failed, error code %d.\n", testResult);
                        } else {
                            printf("AutoTest finished successfully.\n");
                        }
                        break;
                }
            }
        } 
        else {
            printf("Invalid selection!\n");
        }

        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
    }

    printf("You exit the program.\n");
    // If I exit I need to clean up by deleting the image 
    if (image) {
        DeleteImage(image);
    }
    return 0; 
#endif
}

void PrintMenu(void) {
    printf("\n--------------------------------\n");
    printf("Please make your choice:\n");
    printf(" 1: Load a PPM image\n");
    printf(" 2: Save an image in PPM and JPEG format\n");
    printf(" 3: Change a color image to Black & White\n");
    printf(" 4: Make a negative of an image\n");
    printf(" 5: Color filter an image\n");
    printf(" 6: Sketch the edge of an image\n");
    printf(" 7: Shuffle an image\n");
    printf(" 8: Flip an image vertically\n");
    printf(" 9: Mirror an image horizontally\n");
    printf("10: Add border to an image\n");
    printf("11: Pixelate an image\n");
    printf("12: Shift an image\n");
    printf("13: Crop an image\n");
    printf("14: Resize an image\n");
    printf("15: Add Watermark to an image\n");
    printf("16: Rotate an image by 90 degrees\n");
    printf("17: Test all functions\n");
    printf("18: Exit\n");
}
