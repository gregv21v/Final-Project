/*
	Color.h
*/

#pragma once

struct Color
{
	float red, green, blue, alpha;

	void setColor(float r, float g, float b, float a)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
};



