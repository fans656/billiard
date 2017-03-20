#include "utils.h"

void draw (IplImage *src, IplImage *des, int x, int y)
{
	for (int i = 0; i < src->height && 0 <= y + i && y + i < des->height; i++) {
		for (int j = 0; j < src->width && 0 <= x + j && x + j < des->width; j++) {
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3)
				= CV_IMAGE_ELEM(src, uchar, i, j * 3);
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3 + 1)
				= CV_IMAGE_ELEM(src, uchar, i, j * 3 + 1);
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3 + 2)
				= CV_IMAGE_ELEM(src, uchar, i, j * 3 + 2);
		}
	}
}

void draw (IplImage *src, IplImage *des, int x, int y, CvScalar& mask)
{
	for (int i = 0; i < src->height && 0 <= y + i && y + i < des->height; i++) {
		for (int j = 0; j < src->width && 0 <= x + j && x + j < des->width; j++) {
			uchar r, g, b;
			g = CV_IMAGE_ELEM(src, uchar, i, j * 3);
			b = CV_IMAGE_ELEM(src, uchar, i, j * 3 + 1);
			r = CV_IMAGE_ELEM(src, uchar, i, j * 3 + 2);
			if (r == mask.val[0] && g == mask.val[1] && b == mask.val[2])
				continue;
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3) = g;
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3 + 1) = b;
			CV_IMAGE_ELEM(des, uchar, y + i, (x + j) * 3 + 2) = r;
		}
	}
}