# Game Architecture homework4: GUI and model loading
Fourth homework for Game Architecture.

In this assignment you will implement three simple GUI widgets: label, button,
and checkbox to create a simple model viewer program.

This has several parts: 
1.  implement the GUI widgets.
2.  load  models using the Asset Importer library 
3. set up the GUI functions to show different models, and to turn on and off lighting in render settings

For 10% Extra Credit: implement the Arcball method for rotating a model, as described by Ken Shoemake in the attached PDF.  
This will require use of a quaternion class, which is added to the math/ directory.  The matrix class additionally has a helper function 
added to construct a rotation transform matrix from a quaternion.  


Some supporting code for the GUI is already written:

	1. Code in ga_input grabs mouse position and button information and stores
	it in ga_frame_params.
	2. Code in ga_font does text rasterization from TrueType using stb_truetype.
	3. Code in ga_material implements untextured, vertex colored geo, as well as material with basic lighting
	4. Code in ga_output draws so called 'dynamic' geometry with screen space
	orthographic projection.

Your job is to implement all the "// TODO: Homework 4" elements in the gui directory,
in  model_component.cpp and in main.cpp.

Some snippets of code that you might find useful follow. Code to emit a text
drawcall:

	extern ga_font* g_font;
	g_font->print(params, text_to_draw, position_x, position_y, color, &min, &max);

Code to emit an outline box draw:

	ga_dynamic_drawcall drawcall;

	drawcall._positions.push_back({ upper_left_x, upper_left_y, 0.0f });
	drawcall._positions.push_back({ lower_right_x, upper_left_y, 0.0f });
	drawcall._positions.push_back({ lower_right_x, lower_right_y, 0.0f });
	drawcall._positions.push_back({ upper_left_x, lower_right_y, 0.0f });
	
	drawcall._indices.push_back(0);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(1);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(2);
	drawcall._indices.push_back(3);
	drawcall._indices.push_back(3);
	drawcall._indices.push_back(0);
	
	drawcall._color = color;
	drawcall._draw_mode = GL_LINES;
	drawcall._transform.make_identity();
	drawcall._material = nullptr;
	
	while (params->_gui_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
	params->_gui_drawcalls.push_back(drawcall);
	params->_gui_drawcall_lock.clear(std::memory_order_release);

Code to detect a mouse click in a region:

	ga_frame_params* params;
	bool click_in_region =
		params->_mouse_click_mask != 0 &&
		params->_mouse_x >= upper_left_x &&
		params->_mouse_y >= upper_left_y &&
		params->_mouse_x <= lower_right_x &&
		params->_mouse_y <= lower_right_y;
