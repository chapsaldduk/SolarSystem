#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

Image::~Image() {
	delete[] pixels;
}

namespace {


	int strToInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	short strToShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	int filtToStr(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return strToInt(buffer);
	}

	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return strToShort(buffer);
	}

	template<class T>
	class bmpArrHelper {
	private:
		T * array;
		mutable bool isReleased;
	public:
		explicit bmpArrHelper(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}

		bmpArrHelper(const bmpArrHelper<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		~bmpArrHelper() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}

		T* get() const {
			return array;
		}

		T &operator*() const {
			return *array;
		}

		void operator=(const bmpArrHelper<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		T* operator->() const {
			return array;
		}

		T* release() {
			isReleased = true;
			return array;
		}

		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}

		T* operator+(int i) {
			return array + i;
		}

		T &operator[](int i) {
			return array[i];
		}
	};
}

Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"file didnot exists");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"only bmp");
	input.ignore(8);
	int dataOffset = filtToStr(input);

	int headerSize = filtToStr(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		width = filtToStr(input);
		height = filtToStr(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"fail");
		assert(readShort(input) == 0 || !"success");
		break;
	case 12:
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"fail");
		break;
	case 64:
	case 108:
	case 124:
	default:
		assert(!"fail");
	}
	//Read the data
	int bytToRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytToRow * height;
	bmpArrHelper<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	bmpArrHelper<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] = pixels[bytToRow * y + 3 * x + (2 - c)];
			}
		}
	}
	input.close();
	return new Image(pixels2.release(), width, height);
}