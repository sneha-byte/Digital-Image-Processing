/*********************************************************************/
/* FileIO.c: program file for I/O module       	                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Constants.h"
#include "FileIO.h"

Image *LoadImage(const char *fname)
{
	FILE         *File;
	char         Type[SLEN];
	int          W, H, MaxValue;
	unsigned int x, y;
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	Image        *image;
	strcpy(fname_tmp, fname);
	strcat(fname_tmp, ftype);
	File = fopen(fname_tmp, "r");
	if (!File) {
#ifdef DEBUG
		printf("\nCan't open file \"%s\" for reading!\n", fname);
#endif
		return NULL;
	}

	fscanf(File, "%79s", Type);
	if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
#ifdef DEBUG
		printf("\nUnsupported file format!\n");
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &W);
	if (W <= 0) {
#ifdef DEBUG
		printf("\nUnsupported image width %d!\n", W);
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &H);
	if (H <= 0) {
#ifdef DEBUG
		printf("\nUnsupported image height %d!\n", H);
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &MaxValue);
	if (MaxValue != 255) {
#ifdef DEBUG
		printf("\nUnsupported image maximum value %d!\n", MaxValue);
#endif
		fclose(File);
		return NULL;
	}
	if ('\n' != fgetc(File)) {
#ifdef DEBUG
		printf("\nCarriage return expected at the end of the file!\n");
#endif
		fclose(File);
		return NULL;
	}

	image = CreateImage(W, H);

	if (!image) {
#ifdef DEBUG
		printf("\nError creating image from %s!\n", fname_tmp);
#endif
		fclose(File);
		return NULL;
	}
	else {
		for (y = 0; y < ImageHeight(image); y++)
			for (x = 0; x < ImageWidth(image); x++) {
				SetPixelR(image, x, y, fgetc(File));
				SetPixelG(image, x, y, fgetc(File));
				SetPixelB(image, x, y, fgetc(File));
			}

		if (ferror(File)) {
#ifdef DEBUG
			printf("\nFile error while reading from file!\n");
#endif
			DeleteImage(image);
			return NULL;
		}

#ifdef DEBUG
		printf("%s was read successfully!\n", fname_tmp);
#endif
		fclose(File);
		return image;
	}
}

int SaveImage(const char *fname, const Image *image)
{
    assert(image != NULL && "No image to save!\n");
	FILE         *File;
	int          x, y;
	char         SysCmd[SLEN * 5];
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	char         fname_tmp2[SLEN];
	unsigned int WIDTH = ImageWidth(image);
	unsigned int HEIGHT = ImageHeight(image);
	strcpy(fname_tmp, fname);
	strcpy(fname_tmp2, fname);
	strcat(fname_tmp2, ftype);

	File = fopen(fname_tmp2, "w");
	if (!File) {
#ifdef DEBUG
		printf("\nCan't open file \"%s\" for writing!\n", fname);
#endif
		return 1;
	}
	fprintf(File, "P6\n");
	fprintf(File, "%d %d\n", WIDTH, HEIGHT);
	fprintf(File, "255\n");

	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			fputc(GetPixelR(image, x, y), File);
			fputc(GetPixelG(image, x, y), File);
			fputc(GetPixelB(image, x, y), File);
		}

	if (ferror(File)) {
#ifdef DEBUG
		printf("\nError while writing to file!\n");
#endif
		return 2;
	}
	fclose(File);
#ifdef DEBUG
	printf("%s was saved successfully. \n", fname_tmp2);
#endif
	/*
	 * Rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
	 * and make it world readable
	 */
	sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw4.tcsh %s",
			fname_tmp2);
	if (system(SysCmd) != 0) {
#ifdef DEBUG
		printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
#endif
		return 3;
	}
#ifdef DEBUG
	printf("%s.jpg was stored for viewing. \n", fname_tmp);
#endif
	return 0;
}
