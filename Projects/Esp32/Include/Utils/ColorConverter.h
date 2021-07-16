#pragma once

#include <cstdint>
#include "HalCommon.h"

namespace Utilities
{

using Hal::LedHsv;
using Hal::Led;

	class ColorConverter
	{
	public:
		ColorConverter();
		~ColorConverter();

		static constexpr uint8_t MaxSaturation = 255;
		static constexpr uint8_t MaxHue = 255;
		static constexpr uint8_t MaxValue = 255;

		static void RgbToHsv(const unsigned char &src_r, const unsigned char &src_g, const unsigned char &src_b,
													uint16_t &dst_h, unsigned char &dst_s, unsigned char &dst_v);

		static void HsvToRgb(const uint16_t &src_h, const unsigned char &src_s, const unsigned char &src_v,
												unsigned char &dst_r, unsigned char &dst_g, unsigned char &dst_b);

		static void HsvToRgb(const LedHsv &ledHsv, Led &LedRgb);

		static void RgbToHsv(const Led &LedRgb, LedHsv &ledHsv);
	};

} // namespace Utilities