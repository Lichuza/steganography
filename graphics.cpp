#include "graphics.h"


ifstream graphics::unlockBMP()
{
	// открываем контейнер
	ifstream fileStream(way1, ifstream::binary);
	if (!fileStream)
	{
		cout << "Error opening file '" << "C:/111.bmp" << "'." << endl;
	}
	return fileStream;
}


void graphics::checkVersion()
{
	fileStream = unlockBMP();
	// заголовк изображения
	
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

	if (fileHeader.bfType != 0x4D42) {
		std::cout << "Error: '" << "C:/111.bmp" << "' is not BMP file." << std::endl;
	}

	// информация изображения
	
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}

	// получаем информацию о битности
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}

	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;

	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}

	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;

	if (fileInfoHeader.biSize >= 52) {
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}

	// если маска не задана, то ставим маску по умолчанию
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}

	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}

	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}

	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}

	// проверка на поддерку этой версии формата
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
		std::cout << "Error: Unsupported BMP format." << std::endl;
	}

	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
		std::cout << "Error: Unsupported BMP bit count." << std::endl;
	}

	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
		std::cout << "Error: Unsupported BMP compression." << std::endl;
	}
}
void graphics::readBMP()
{
	rgbInfo = new RGBQUAD*[fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;


	// чтение
	unsigned int bufer;

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
			rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
			rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
			rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
			rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
		}
		fileStream.seekg(linePadding, ios_base::cur);
	}
	fileStream.close();
}

void graphics::writeBMP()
{
	FILE *oFile = fopen(way2, "wb");
	// записываем заголовок файла
	write_u16(fileHeader.bfType, oFile);
	write_u32(fileHeader.bfSize, oFile);
	write_u16(fileHeader.bfReserved1, oFile);
	write_u16(fileHeader.bfReserved2, oFile);
	write_u32(fileHeader.bfOffBits, oFile);

	// записываем заголовок изображения
	if (fileInfoHeader.biSize >= 40)
	{
		write_u32(fileInfoHeader.biSize, oFile);
		write_s32(fileInfoHeader.biWidth, oFile);
		write_s32(fileInfoHeader.biHeight, oFile);
		write_u16(fileInfoHeader.biPlanes, oFile);
		write_u16(fileInfoHeader.biBitCount, oFile);
		write_u32(fileInfoHeader.biCompression, oFile);
		write_u32(fileInfoHeader.biSizeImage, oFile);
		write_s32(fileInfoHeader.biXPelsPerMeter, oFile);
		write_s32(fileInfoHeader.biYPelsPerMeter, oFile);
		write_u32(fileInfoHeader.biClrUsed, oFile);
		write_u32(fileInfoHeader.biClrImportant, oFile);
	}

	if (fileInfoHeader.biSize >= 52)
	{
		write_u32(fileInfoHeader.biRedMask, oFile);
		write_u32(fileInfoHeader.biGreenMask, oFile);
		write_u32(fileInfoHeader.biBlueMask, oFile);
	}
	
	if (fileInfoHeader.biSize >= 56)
	{
		write_u32(fileInfoHeader.biAlphaMask, oFile);
	}
	
	if (fileInfoHeader.biSize >= 108)
	{
		write_u32(fileInfoHeader.biCSType, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzRed.ciexyzX, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzRed.ciexyzY, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzRed.ciexyzZ, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzGreen.ciexyzX, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzGreen.ciexyzY, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzGreen.ciexyzZ, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzBlue.ciexyzX, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzBlue.ciexyzY, oFile);
		write_s32(fileInfoHeader.biEndpoints.ciexyzBlue.ciexyzX, oFile);
		write_u32(fileInfoHeader.biGammaRed, oFile);
		write_u32(fileInfoHeader.biGammaGreen, oFile);
		write_u32(fileInfoHeader.biGammaBlue, oFile);
	}

	if (fileInfoHeader.biSize >= 124) 
	{
		write_u32(fileInfoHeader.biIntent, oFile);
        write_u32(fileInfoHeader.biProfileData, oFile);
        write_u32(fileInfoHeader.biProfileSize, oFile);
        write_u32(fileInfoHeader.biReserved, oFile);
	}


	// вывод
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {	
			putc(rgbInfo[i][j].rgbBlue, oFile);
			putc(rgbInfo[i][j].rgbGreen, oFile);
			putc(rgbInfo[i][j].rgbRed, oFile);
			//putc(rgbInfo[i][j].rgbReserved, oFile);
		}
	}
	fclose(oFile);
}


unsigned short graphics::read_u16(FILE *fp)
{
	unsigned char b0, b1;

	b0 = getc(fp);
	b1 = getc(fp);

	return ((b1 << 8) | b0);
}


unsigned int graphics::read_u32(FILE *fp)
{
	unsigned char b0, b1, b2, b3;

	b0 = getc(fp);
	b1 = getc(fp);
	b2 = getc(fp);
	b3 = getc(fp);

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


int graphics::read_s32(FILE *fp)
{
	unsigned char b0, b1, b2, b3;

	b0 = getc(fp);
	b1 = getc(fp);
	b2 = getc(fp);
	b3 = getc(fp);

	return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

void graphics::write_u16(unsigned short input, FILE *fp)
{
	putc(input, fp);
	putc(input >> 8, fp);
}

void graphics::write_u32(unsigned int input, FILE *fp)
{
	putc(input, fp);
	putc(input >> 8, fp);
	putc(input >> 16, fp);
	putc(input >> 24, fp);
}

void graphics::write_s32(int input, FILE *fp)
{
	putc(input, fp);
	putc(input >> 8, fp);
	putc(input >> 16, fp);
	putc(input >> 24, fp);
}

unsigned char graphics::bitextract(const unsigned int byte, const unsigned int mask) {
	if (mask == 0) {
		return 0;
	}

	// определение количества нулевых бит справа от маски
	int
		maskBufer = mask,
		maskPadding = 0;

	while (!(maskBufer & 1)) {
		maskBufer >>= 1;
		maskPadding++;
	}

	// применение маски и смещение
	return (byte & mask) >> maskPadding;
}

void graphics::packingDW()
{
	int h = 0;
	
	while (k.size() != 1920)
	{
	skip1: int randI = rand() % 1920 + 0;

		for (int i = 0; i < k.size(); i++)
		{
			if ((k[i] == randI)) goto skip1;
		}
		k.push_back(randI);
	}

    ifstream fileStream(way3, ifstream::binary);
	while (fileStream)
	{
		char * buffer = new char[8];
		fileStream.read(buffer, 1);

		bitset<8> bitF = *buffer;

		for (int j = 0; j < 8; j++)
		{
			int tempK = k[i1];
			std::bitset<8> bitB = rgbInfo[h][tempK].rgbBlue;
			bitB[0] = bitF[j];
			std::bitset<8> bitset1(bitB.to_string());
			rgbInfo[h][tempK].rgbBlue = bitset1.to_ulong();
			if (h < 1079)h++;
			else h = 0;
		}
		i1++;
	}
	fileStream.close();
}


void graphics::extractionDW(vector<int> k, const int i1)
{
    ofstream wDW(way3, ofstream::binary);
	char * buffer = new char[8];
	int h = 0;
	for (int i = 0; i < i1; i++)
	{
		int tempk = k[i];

		int j = 0;
		std::bitset<8> bitDW;
		while (j < 8) 
		{
			std::bitset<8> bitB = rgbInfo[h][tempk].rgbBlue;		
			bitDW[j] = bitB[0];
			if (h < 1079)h++;
			else h = 0;		
			j++;
		}
		*buffer = bitDW.to_ulong();
		wDW.write(buffer, 1);
	}
	wDW.close();
}

double graphics::assessmentMSE(const string way1, const string way2)
{
	ifstream originalDW(way1, ofstream::binary);
	ifstream extractionDW(way2, ofstream::binary);
	char * buffer1 = new char[1];
	char * buffer2 = new char[1];

	double h = 0;
	double sum = 0;
	while (originalDW) 
	{
		originalDW.read(buffer1, 1);
		extractionDW.read(buffer2, 1);

		std::bitset<8> temp1 = *buffer1;
		std::bitset<8> temp2 = *buffer2;
		for (int i = 0; i < 8; i++)
		{
			h++;
			if (temp1[i] == temp2[i]) continue;
			else sum++;
		}		
	}
	return sum / h;
}

double graphics::assessmentSNR(const string way1, const string way2)
{
	ifstream originalDW(way1, ofstream::binary);
	ifstream extractionDW(way2, ofstream::binary);
	char * buffer1 = new char[1];
	char * buffer2 = new char[1];

	double sum = 0;
	double n = 0;
	while (originalDW)
	{
		originalDW.read(buffer1, 1);
		extractionDW.read(buffer2, 1);

		std::bitset<8> temp1 = *buffer1;
		std::bitset<8> temp2 = *buffer2;
		for (int i = 0; i < 8; i++)
		{
			if (temp1[i] == temp2[i]) sum++;
			else n++;
		}
	}
	return sum / n;
}

double graphics::assessmentNMSE(const string way1, const string way2)
{
	ifstream originalDW(way1, ofstream::binary);
	ifstream extractionDW(way2, ofstream::binary);
	char * buffer1 = new char[1];
	char * buffer2 = new char[1];

	double h = 0;
	double sum = 0;
	while (originalDW)
	{
		originalDW.read(buffer1, 1);
		extractionDW.read(buffer2, 1);

		std::bitset<8> temp1 = *buffer1;
		std::bitset<8> temp2 = *buffer2;
		for (int i = 0; i < 8; i++)
		{
			h+= temp1[i];
			if (temp1[i] == temp2[i]) continue;
			else sum++;
		}
	}
	return sum / h;
}
