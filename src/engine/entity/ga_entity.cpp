/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_entity.h"
#include "ga_component.h"

ga_entity::ga_entity()
{
	_transform.make_identity();
	_active = true;

}

ga_entity::~ga_entity()
{
}

void ga_entity::add_component(ga_component* comp)
{
	_components.push_back(comp);
}

void ga_entity::update(ga_frame_params* params)
{
	if (_active)
	{
		for (auto& c : _components)
		{
			c->update(params);
		}
	}
}

void ga_entity::late_update(ga_frame_params* params)
{
	if (_active)
	{
		for (auto& c : _components)
		{
			c->late_update(params);
		}
	}
}

void ga_entity::translate(const ga_vec3f& translation)
{
	_transform.translate(translation);
}
void ga_entity::scale(const float& scaling)
{
	_transform.scale(scaling);
}

void ga_entity::rotate(const ga_vec3f& rotation)
{
	ga_mat4f rotation_matrix;
	rotation_matrix.make_identity();

	rotation_matrix.rotate_y(ga_degrees_to_radians(rotation.y));
	rotation_matrix.rotate_z(ga_degrees_to_radians(rotation.z));
	rotation_matrix.rotate_x(ga_degrees_to_radians(rotation.x));

	_transform = rotation_matrix * _transform;
}

/*
void ga_entity::rotate(const ga_quatf& rotation)
{
	ga_mat4f rotation_m;
	rotation_m.make_rotation(rotation);
	_transform = rotation_m * _transform;
}
*/
