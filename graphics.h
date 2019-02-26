#define _CRT_SECURE_NO_WARNINGS
#define MAIN_H_INCLUDED
#include <iostream>
#include <fstream>
#include<vector>
#include <bitset>
using namespace std;

	class graphics
	{
	public:
        graphics(const string wayContainer1, const char *wayContainer2, string wayDW) { way1 = wayContainer1, way2 = wayContainer2, way3 = wayDW; }
        graphics(const string wayContainer2, const string wayOutDW) { way1 = wayContainer2, way3 = wayOutDW; }


		// CIEXYZTRIPLE stuff
		typedef int FXPT2DOT30;

		typedef struct {
			FXPT2DOT30 ciexyzX;
			FXPT2DOT30 ciexyzY;
			FXPT2DOT30 ciexyzZ;
		} CIEXYZ;

		typedef struct {
			CIEXYZ  ciexyzRed;
			CIEXYZ  ciexyzGreen;
			CIEXYZ  ciexyzBlue;
		} CIEXYZTRIPLE;

		// bitmap file header
		typedef struct {
			unsigned short bfType;
			unsigned int   bfSize;
			unsigned short bfReserved1;
			unsigned short bfReserved2;
			unsigned int   bfOffBits;
		} BITMAPFILEHEADER;

		// bitmap info header
		typedef struct {
			unsigned int   biSize;
			unsigned int   biWidth;
			unsigned int   biHeight;
			unsigned short biPlanes;
			unsigned short biBitCount;
			unsigned int   biCompression;
			unsigned int   biSizeImage;
			unsigned int   biXPelsPerMeter;
			unsigned int   biYPelsPerMeter;
			unsigned int   biClrUsed;
			unsigned int   biClrImportant;
			unsigned int   biRedMask;
			unsigned int   biGreenMask;
			unsigned int   biBlueMask;
			unsigned int   biAlphaMask;
			unsigned int   biCSType;
			CIEXYZTRIPLE   biEndpoints;
			unsigned int   biGammaRed;
			unsigned int   biGammaGreen;
			unsigned int   biGammaBlue;
			unsigned int   biIntent;
			unsigned int   biProfileData;
			unsigned int   biProfileSize;
			unsigned int   biReserved;
		} BITMAPINFOHEADER;

		// rgb quad
		typedef struct {
			unsigned char  rgbBlue;
			unsigned char  rgbGreen;
			unsigned char  rgbRed;
			unsigned char  rgbReserved;
		} RGBQUAD;



		// read bytes
		template <typename Type>
		void read(std::ifstream &fp, Type &result, size_t size) {
			fp.read(reinterpret_cast<char*>(&result), size);
		}

		// bit extract
		unsigned char bitextract(const unsigned int byte, const unsigned int mask);


		static unsigned short read_u16(FILE *fp);
		static unsigned int   read_u32(FILE *fp);
		static int            read_s32(FILE *fp);

		static void write_u16(unsigned short input, FILE *fp);
		static void write_u32(unsigned int input, FILE *fp);
		static void write_s32(int input, FILE *fp);

		ifstream unlockBMP();
		void checkVersion();
		void readBMP();
		void writeBMP();
		void packingDW();
		void extractionDW(vector<int> k, const int i1);

		vector<int> &getkey() { return k; }
		int getI() { return i1; }

		double assessmentMSE(const string way1, const string way2);
		double assessmentNMSE(const string way1, const string way2);
		double assessmentSNR(const string way1, const string way2);

		
	private:
        string way1, way3;
		const char *way2;
		ifstream fileStream;
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER fileInfoHeader;
		// rgb info
		RGBQUAD **rgbInfo;
		vector<int> k;
		int i1 = 0;
	};




