#include "stdafx.h"

#include <string>
#include <clocale>
#include <locale>
#include <vector>
#include <codecvt>

//2020july04, spi, begin
#include "utility.h"
//2020july04, spi, end

//2020august14, spi, begin
//static bool first = true;
static bool first = false;
//2020august14, spi, end
int random_int(int min, int max) //range : [min, max)
{
	//static bool first = true;
	if (first)
	{
		//2020july03, spi, begin
		//srand(time(NULL)); //seeding for the first time only!
		srand(GetTickCount()); //seeding for the first time only!
		//2020july03, spi, end
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}


std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	//using convert_typeX = std::codecvt_utf16<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	//using convert_typeX = std::codecvt_utf16<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}


// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


//2020july04, spi, begin
//RGB to HSV and HSV to RGB conversions from:
//https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//note: code without floats (faster but less accurate):

/* //defined in utility.h
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
*/

RgbColor HsvToRgb(HsvColor hsv)
{
	RgbColor rgb;
	unsigned char region, remainder, p, q, t;

	if (hsv.s == 0)
	{
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;

	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb.r = hsv.v; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = hsv.v; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = hsv.v; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = hsv.v;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = hsv.v;
		break;
	default:
		rgb.r = hsv.v; rgb.g = p; rgb.b = q;
		break;
	}

	return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
	HsvColor hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}

bool IsInHSVColorRange(HsvColor myHsvColor, int hsvcolorrangetype)
{
	//UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN	
	//UTILITY_HSVCOLORRANGE_TYPE_RED			
	//UTILITY_HSVCOLORRANGE_TYPE_BLUE			
	//UTILITY_HSVCOLORRANGE_TYPE_YELLOW
	if (hsvcolorrangetype==UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN)
	{
		/*
		//# define the upper and lower boundaries of the HSV pixel
		//# intensities to be considered 'skin'
		//lower = np.array([0, 48, 80], dtype = "uint8")
		//upper = np.array([20, 255, 255], dtype = "uint8")
		if ( (myHsvColor.h >=0) && (myHsvColor.h <= 20) )
		{
			if ((myHsvColor.s >= 48) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 80) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
		*/
		//alternatively, from:
		//https://stackoverflow.com/questions/8753833/exact-skin-color-hsv-range#:~:text=The%20skin%20in%20channel%20H,for%20small%20values%20of%20V.
		//lower_color = np.array([108, 23, 82])
		//upper_color = np.array([179, 255, 255])
		/*
		if ((myHsvColor.h >= 108) && (myHsvColor.h <= 179))
		{
			if ((myHsvColor.s >= 23) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 82) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
		*/
		/*
		//[0, 58, 50] lower bound skin HSV
		//[30, 255, 255] upper bound skin HSV
		if ((myHsvColor.h >= 0) && (myHsvColor.h <= 30))
		{
			if ((myHsvColor.s >= 58) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 50) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
		*/
		/*
		//[0, 58, 50] lower bound skin HSV
		//[30, 174, 255] upper bound skin HSV
		if ((myHsvColor.h >= 0) && (myHsvColor.h <= 30))
		{
			if ((myHsvColor.s >= 58) && (myHsvColor.s <= 174))
			{
				if ((myHsvColor.v >= 50) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
		*/
		//spi
		if ( ((myHsvColor.h >= 0) && (myHsvColor.h <= 30)) || ((myHsvColor.h >= 108) && (myHsvColor.h <= 179)) )
		{
			if ((myHsvColor.s >= 23) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 50) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}

	}
	else if (hsvcolorrangetype == UTILITY_HSVCOLORRANGE_TYPE_RED)
	{
		//https://stackoverflow.com/questions/32522989/opencv-better-detection-of-red-color
		if ( ((myHsvColor.h >= 0) && (myHsvColor.h <= 10)) || ((myHsvColor.h >= 170) && (myHsvColor.h <= 180)))
		{
			if ((myHsvColor.s >= 70) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 50) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
	}
	else if (hsvcolorrangetype == UTILITY_HSVCOLORRANGE_TYPE_GREEN)
	{
		//https://stackoverflow.com/questions/31590499/opencv-android-green-color-detection
		//Green color is HSV space has H = 120 and it's in range [0, 360].
		//OpenCV halves the H values to fit the range [0,255], so H value instead of being in range [0, 360], is in range [0, 180]. S and V are still in range [0, 255].
		//As a consequence, the value of H for green is 60 = 120 / 2.
		// sensitivity is a int, typically set to 15 - 20 
		int sensitivity = 15;
		if ((myHsvColor.h >= (60-sensitivity)) && (myHsvColor.h <= (60+sensitivity)))
		{
			if ((myHsvColor.s >= 100) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 50) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
	}
	else if (hsvcolorrangetype == UTILITY_HSVCOLORRANGE_TYPE_BLUE)
	{
		//https://answers.opencv.org/question/90047/detecting-blue-color-in-this-image/
		if ((myHsvColor.h >= 100) && (myHsvColor.h <= 140))
		{
			if ((myHsvColor.s >= 150) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 0) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
	}
	else if (hsvcolorrangetype == UTILITY_HSVCOLORRANGE_TYPE_YELLOW)
	{
		//https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
		if ((myHsvColor.h >= 20) && (myHsvColor.h <= 30))
		{
			if ((myHsvColor.s >= 100) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 100) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
	}
	else if (hsvcolorrangetype == UTILITY_HSVCOLORRANGE_TYPE_ORANGE)
	{
		//https://realpython.com/python-opencv-color-spaces/
		if ((myHsvColor.h >= 1) && (myHsvColor.h <= 18))
		{
			if ((myHsvColor.s >= 190) && (myHsvColor.s <= 255))
			{
				if ((myHsvColor.v >= 200) && (myHsvColor.v <= 255))
				{
					//if 3 conditions are meet, hsv color is in range
					return true;
				}
			}
		}
	}
	return false;
}

//2020july04, spi, end
