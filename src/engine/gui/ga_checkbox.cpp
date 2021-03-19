/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_checkbox.h"
#include "ga_font.h"

#include "framework/ga_frame_params.h"

ga_checkbox::ga_checkbox(bool state, const char* text, float x, float y, ga_frame_params* params)
{
	// TODO: Homework 4
	extern ga_font* g_font;
	g_font->print(params, text, x+k_checkbox_offset, y, k_text_color, &_min, &_max);

	_min.axes[0] = x;
	_max.axes[0] = x + (_max.axes[1] - _min.axes[1]);
	

	ga_vec3f color = k_button_color;
	if (get_pressed(params) || get_clicked(params)) { color = k_button_press_color; }
	else if (get_hover(params)) { color = k_button_hover_color; }

	draw_outline(params, _min, _max, color, k_button_offset);

	if (state)
	{
		draw_check(params, _min, _max, color, k_button_offset);
	}
}

ga_checkbox::~ga_checkbox()
{
}

bool ga_checkbox::get_hover(const ga_frame_params* params) const
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

bool ga_checkbox::get_pressed(const ga_frame_params* params) const
{
	// TODO: Homework 4
	if (get_hover(params) && params->_mouse_press_mask)
	{
		return true;
	}
	return false;
}

bool ga_checkbox::get_clicked(const ga_frame_params* params) const
{
	// TODO: Homework 4
	if (get_hover(params) && params->_mouse_click_mask)
	{
		return true;
	}
	return false;
}
