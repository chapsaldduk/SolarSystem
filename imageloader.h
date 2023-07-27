#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED
class Image {
	// 이미지 매핑
public:
	Image(char* ps, int w, int h);
	~Image();
	char* pixels;
	int width;
	int height;
};

Image* loadBMP(const char* filename);

#endif
