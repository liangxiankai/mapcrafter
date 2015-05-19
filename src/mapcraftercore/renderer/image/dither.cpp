/*
 * Copyright 2012-2015 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dither.h"

#include "palette.h"
#include "../image.h"

namespace mapcrafter {
namespace renderer {

void imageDither(RGBAImage& image, const Palette& palette, std::vector<int>& data) {
	int width = image.getWidth();
	int height = image.getHeight();
	data.resize(width * height);

	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			RGBAPixel old_color = image.pixel(x, y);
			int color_id = palette.getNearestColor(old_color);
			RGBAPixel new_color = palette.getColors()[color_id];
			image.pixel(x, y) = new_color;
			data[y * width + x] = color_id;

			int error_r = rgba_red(old_color) - rgba_red(new_color);
			int error_g = rgba_green(old_color) - rgba_green(new_color);
			int error_b = rgba_blue(old_color) - rgba_blue(new_color);

			image.setPixel(x+1, y, rgba_add_clamp(image.getPixel(x+1, y), error_r * 7/16, error_g * 7/16, error_b * 7/16));
			image.setPixel(x-1, y+1, rgba_add_clamp(image.getPixel(x-1, y+1), error_r * 3/16, error_g * 3/16, error_b * 3/16));
			image.setPixel(x, y+1, rgba_add_clamp(image.getPixel(x, y+1), error_r * 5/16, error_g * 5/16, error_b * 5/16));
		}
	}
}

}
}