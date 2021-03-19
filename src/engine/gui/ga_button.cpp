/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_button.h"
#include "ga_font.h"

#include "framework/ga_frame_params.h"

ga_button::ga_button(const char* text, float x, float y, ga_frame_params* params)
{
	// TODO: Homework 4
	// hint, first draw the text, then decide what else to draw based on the get functions below

	extern ga_font* g_font;
	g_font->print(params, text, x, y, k_text_color, &_min, &_max);


	ga_vec3f color = k_button_color;
	if (get_pressed(params) || get_clicked(params)) 
	{ 
		color = k_button_press_color; 
		draw_fill(params, _min, _max, color, k_button_offset);
	}
	else if (get_hover(params)) { color = k_button_hover_color; }

	draw_outline(params, _min, _max, color, k_button_offset);
	g_font->print(params, text, x, y, k_text_color, &_min, &_max);
}

ga_button::~ga_button()
{
}

bool ga_button::get_hover(const ga_frame_params* params) const
{
	// TODO: Homework 4
	float min_x = _min.axes[0] - k_button_offset;
	float min_y = _min.axes[1] - k_button_offset;
	float max_x = _max.axes[0] + k_button_offset;
	float max_y = _max.axes[1] + k_button_offset;
	if (params->_mouse_x >= min_x &&
		params->_mouse_x <= max_x &&
		params->_mouse_y >= min_y &&
		params->_mouse_y <= max_y)
	{
		return true;
	}
	return false;
}

bool ga_button::get_pressed(const ga_frame_params* params) const
{
	// TODO: Homework 4
	if (get_hover(params) && params->_mouse_press_mask) 
	{ 
		return true; 
	}
	return false;
}

bool ga_button::get_clicked(const ga_frame_params* params) const
{
	// TODO: Homework 4
	if (get_hover(params) && params->_mouse_click_mask)
	{ 
		return true; 
	}
	return false;
}
