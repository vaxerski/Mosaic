#pragma once

struct float3 {
	float x, y, z;
};

namespace math {
	float3 hsv2rgb(float hue, float sat, float val);
	float3 rgb2hsv(float red, float grn, float blu);
}

