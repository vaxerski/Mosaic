#include <stdio.h>
#include <math.h>
#include "math.h"
#include <cmath>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

float3 math::hsv2rgb(float hue, float sat, float val) {
	float red, grn, blu;
	float i, f, p, q, t;

	if (val == 0) {
		red = 0;
		grn = 0;
		blu = 0;
	}
	else {
		hue /= 60;
		i = floor(hue);
		f = hue - i;
		p = val * (1 - sat);
		q = val * (1 - (sat * f));
		t = val * (1 - (sat * (1 - f)));
		if (i == 0) { red = val; grn = t; blu = p; }
		else if (i == 1) { red = q; grn = val; blu = p; }
		else if (i == 2) { red = p; grn = val; blu = t; }
		else if (i == 3) { red = p; grn = q; blu = val; }
		else if (i == 4) { red = t; grn = p; blu = val; }
		else if (i == 5) { red = val; grn = p; blu = q; }
	}
	float3 result;
	result.x = red;
	result.y = grn;
	result.z = blu;
	return result;
}

float3 math::rgb2hsv(float red, float grn, float blu) {
	float hue, sat, val;
	float x, f, i;

	x = MIN(MIN(red, grn), blu);
	val = MAX(MAX(red, grn), blu);
	if (x == val) {
		hue = 0;
		sat = 0;
	}
	else {
		f = (red == x) ? grn - blu : ((grn == x) ? blu - red : red - grn);
		i = (red == x) ? 3 : ((grn == x) ? 5 : 1);
		hue = fmod((i - f / (val - x)) * 60, 360);
		sat = ((val - x) / val);
	}
	float3 result;
	result.x = hue;
	result.y = sat;
	result.z = val;
	return result;
}
