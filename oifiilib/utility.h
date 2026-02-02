#pragma once

#include <string>

int random_int(int min, int max); //range : [min, max)

std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);

std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);


//2020july04, spi, begin
//RGB to HSV and HSV to RGB conversions from:
//https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//note: code without floats (faster but less accurate):

typedef struct RgbColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RgbColor;

typedef struct HsvColor
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv);
HsvColor RgbToHsv(RgbColor rgb);

#define UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN	0
#define UTILITY_HSVCOLORRANGE_TYPE_RED			1
#define UTILITY_HSVCOLORRANGE_TYPE_GREEN		2
#define UTILITY_HSVCOLORRANGE_TYPE_BLUE			3
#define UTILITY_HSVCOLORRANGE_TYPE_YELLOW		4
#define UTILITY_HSVCOLORRANGE_TYPE_ORANGE		5

bool IsInHSVColorRange(HsvColor myHsvColor, int hsvcolorrangetype);

//2020july04, spi, end
