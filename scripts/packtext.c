#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "./stbi_image_write.h"

typedef struct Image {
	int width;
	int height;
	int nChannels;
	uint8_t* data;
} Image;

void read(char* fileName, Image* image) {
	uint8_t* data = stbi_load(fileName, &image->width, &image->height, &image->nChannels, 0);

	if (data == NULL) {
		fprintf(stderr, "Could not read file %s\n", fileName);
		exit(1);
	}

	image->data = data;
}

int main(int argc, char** argv) {

	if (argc < 3) {
		fprintf(stderr, "usage: %s image1 image2 [...image]\n", argv[0]);
		exit(1);
	}

	int numImages = argc - 1;
	Image finalImage;

	Image image;
	read(argv[1], &image);
	finalImage.width = image.width;
	finalImage.height = image.height;
	finalImage.nChannels = image.nChannels;


	int imageSize = finalImage.width * finalImage.height * finalImage.nChannels; // Chould be 1 byte per element
	finalImage.data = (uint8_t*) malloc(imageSize * numImages);
	memcpy(finalImage.data, image.data, imageSize);
	stbi_image_free(image.data);

	for (int i = 2; i < argc; i++) {
		read(argv[i], &image);
		if (image.height != finalImage.height) {
			fprintf(stderr, "Height does not match for image %s\n", argv[i]);
			exit(1);
		}
		else if (image.width != finalImage.width) {
			fprintf(stderr, "Width does not match for image %s\n", argv[i]);
			exit(1);
		}
		else if (image.nChannels != finalImage.nChannels) {
			fprintf(stderr, "NumChannels does not match for image %s\n", argv[i]);
			exit(1);
		}

		memcpy(finalImage.data + (imageSize * (i - 1)), image.data, imageSize);
	}

	stbi_write_png("output.png", finalImage.width, numImages * finalImage.height, finalImage.nChannels, finalImage.data, finalImage.width* finalImage.nChannels);

	return 0;
}
