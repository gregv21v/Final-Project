/*
	Color.h
*/

#pragma once

struct Color
{
	float _red, _green, _blue, _alpha;


	void setColor(float red, float green, float blue, float alpha)
	{
		_red = red;
		_green = green;
		_blue = blue; 
		_alpha = alpha;
	}
};



