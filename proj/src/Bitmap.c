/* A maior parte do código de "Bitmap.c" é da autoria de Henrique Ferrolho.
 * Foram feitas algumas alterações à função drawBitmap, para permitir a simulação de transparência.
 * Este código foi retirado da página web: http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */

#include "Bitmap.h"

#include "stdio.h"
#include "video_gr.h"

Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}

void drawBitmap(char * ptr, Bitmap* bmp, int x, int y, Alignment alignment) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > vg_getHorRes() || y + height < 0
			|| y > vg_getVerRes())
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > vg_getHorRes())
			drawWidth = vg_getHorRes();
	} else if (x + drawWidth >= vg_getHorRes()) {
		drawWidth = vg_getHorRes() - x;
	}

	//Changes to Ferrolho's code
	uint16_t* bufferStartPos;
	uint16_t* imgStartPos;

	int i;
	int j;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= vg_getVerRes())
			continue;

		bufferStartPos = (uint16_t*) ptr;
		bufferStartPos += x + pos * vg_getHorRes();

		for (j = 0; j <= width; j++) {
			if (*((uint16_t *) bmp->bitmapData + xCorrection + i * width + j)
					!= TRANSPARENCY) {

				imgStartPos = (uint16_t*) bmp->bitmapData + xCorrection;
				imgStartPos += i * width + j;

				memcpy(bufferStartPos + j, imgStartPos, sizeof(uint16_t));
			}
		}
	}
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}
