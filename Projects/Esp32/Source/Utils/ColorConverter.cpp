#include "ColorConverter.h"
#include <cassert>
#include <cstring>
#include <cstdio>

namespace Utilities
{

void ColorConverter::RgbToHsv(const Led &ledRgb, LedHsv &ledHsv)
{
	RgbToHsv(ledRgb.Color.Red, ledRgb.Color.Green, ledRgb.Color.Blue, ledHsv.Color.Hue, ledHsv.Color.Saturation, ledHsv.Color.Value);
}

void ColorConverter::RgbToHsv(const unsigned char &src_r, const unsigned char &src_g, const unsigned char &src_b, uint16_t &dst_h, unsigned char &dst_s, unsigned char &dst_v)
{
    float r = src_r / 255.0f;
    float g = src_g / 255.0f;
    float b = src_b / 255.0f;

    float h = 0;
	float s = 0; 
	float v = 0; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

    float max = std::max(r , std::max(g, b));
    float min = std::max(r , std::max(g, b));

    v = max;

    if (max == 0.0f) {
        s = 0;
        h = 0;
    }
    else if (max - min == 0.0f) {
        s = 0;
        h = 0;
    }
    else {
        s = (max - min) / max;

        if (max == r) {
            h = 60 * ((g - b) / (max - min)) + 0;
        }
        else if (max == g) {
            h = 60 * ((b - r) / (max - min)) + 120;
        }
        else {
            h = 60 * ((r - g) / (max - min)) + 240;
        }
    }

    if (h < 0) h += 360.0f;

    dst_h = (uint16_t)(h);   // dst_h : 0-180
    dst_s = (unsigned char)(s * 255); // dst_s : 0-255
    dst_v = (unsigned char)(v * 255); // dst_v : 0-255
}

void ColorConverter::HsvToRgb(const LedHsv &ledHsv, Led &LedRgb)
{
	HsvToRgb(ledHsv.Color.Hue, ledHsv.Color.Saturation, ledHsv.Color.Value, LedRgb.Color.Red, LedRgb.Color.Green, LedRgb.Color.Blue);
}

void ColorConverter::HsvToRgb(const uint16_t &src_h, const unsigned char &src_s, const unsigned char &src_v, unsigned char &dst_r, unsigned char &dst_g, unsigned char &dst_b)
{
    float h = (src_h > 360) ? 360 : src_h; // 0-360
    float s = src_s / 255.0f; // 0.0-1.0
    float v = src_v / 255.0f; // 0.0-1.0

    float r = 0;
	float g = 0;
	float b = 0; // 0.0-1.0

    int   hi = (int)(h / 60.0f) % 6;
    float f  = (h / 60.0f) - hi;
    float p  = v * (1.0f - s);
    float q  = v * (1.0f - s * f);
    float t  = v * (1.0f - s * (1.0f - f));

    switch(hi) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    dst_r = static_cast<unsigned char>(r * 255); // dst_r : 0-255
    dst_g = static_cast<unsigned char>(g * 255); // dst_r : 0-255
    dst_b = static_cast<unsigned char>(b * 255); // dst_r : 0-255
}



} // namespace Utilities