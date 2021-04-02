/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_model_component.h"
#include "ga_material.h"

#include "entity/ga_entity.h"

/* assimp include files. These three are usually needed. */

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <cassert>

ga_model_component::ga_model_component(ga_entity* ent, const char* model_file) : ga_component(ent)
{
	extern char g_root_path[256];
	Assimp::Importer importer;
	std::string model_path = g_root_path;
	model_path += model_file;

	_scene = importer.ReadFile(model_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!_scene)
	{
		std::cout << "error: couldn't load obj\n";
	}
	else
	{
		// process the scene
		process_node_recursive(_scene->mRootNode, _scene);
	}
}

ga_model_component::~ga_model_component()
{
	
}

void ga_model_component::process_node_recursive(aiNode* node, const aiScene* scene)
{
	// process my meshes
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		ga_mesh* mesh = new ga_mesh();
		mesh->create_from_aiMesh(_scene->mMeshes[node->mMeshes[i]], _scene);
		_meshes.push_back(mesh);
	}
	// process my children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		process_node_recursive(node->mChildren[i], scene);
	}
}

void ga_model_component::update(ga_frame_params* params)
{
	const float dt = std::chrono::duration_cast<std::chrono::duration<float>>(params->_delta_time).count();
	get_entity()->rotate({ 0,60.0f*dt,0 });

	for (ga_mesh* m : _meshes)
	{
		ga_static_drawcall draw;
		draw._name = "model";
		m->assemble_drawcall(draw);
		draw._transform = get_entity()->get_transform();

		while (params->_static_drawcall_lock.test_and_set(std::memory_order_acquire)) {}
		params->_static_drawcalls.push_back(draw);
		params->_static_drawcall_lock.clear(std::memory_order_release);
	}
}

ga_mesh::ga_mesh()
{
	_index_count = 0;
	_material = nullptr;
	_vao = 0;
}
ga_mesh::~ga_mesh()
{
	glDeleteBuffers(4, _vbo);
	glDeleteVertexArrays(1, &_vao);
}

void ga_mesh::get_vertices(aiMesh* mesh)
{
	_name = mesh->mName.C_Str();

	// 	// TODO: Homework 4
	// load vertex positions, indices, uv's and normals from importer mesh into our data structure

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		// the mesh's mVertices array contains vertex coordinates
		// our _vertex array needs them...
		

		// load _vertex_array from mesh->mVertices
		_vertex_array.push_back({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

		if (mesh->HasTextureCoords(i))
		{
			// transfer _texcoords from mesh equivalent
			_texcoords.push_back({ mesh->mTextureCoords[i]->x,mesh->mTextureCoords[i]->y });
		}
		if (mesh->HasNormals())
		{
			// transfer _normals from mesh equivalent
			_normals.push_back({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		}
	}
	for (int f = 0; f < mesh->mNumFaces; f++)
	{
		// the mesh->mFaces array contains faces made up of indices...
		// our mesh's _index_array should contain ours...
		for (int k = 0; k < mesh->mFaces[f].mNumIndices; k++)
		{
			_index_array.push_back(mesh->mFaces[f].mIndices[k]);
		}
		
	}
	_index_count = _index_array.size();
}

void ga_mesh::make_buffers()
{
	// TODO: Homework 4 
	// set up vertex and element array buffers for positions, indices, uv's and normals
	// things are already in the ga_mesh's arrays...

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	/* Allocate and assign  Vertex Buffer Objects to our handle */
	glGenBuffers(4, _vbo);

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);

	/* Copy the vertex data  to our buffer */
	glBufferData(GL_ARRAY_BUFFER, _vertex_array.size() * sizeof(GLfloat) * 3, _vertex_array.data(), GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);

	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	/* Bind our second VBO as being the active buffer and storing vertex attributes (uv's) */
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	assert(glGetError() == GL_NO_ERROR);

	/* Copy the color data from UVs to our buffer */
	glBufferData(GL_ARRAY_BUFFER, _texcoords.size() * sizeof(GLfloat) * 2, _texcoords.data(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	/* Specify that our uv data is going into attribute index 1, and contains two floats per vertex */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);

	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[2]);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index_array.size() * sizeof(GLushort), _index_array.data(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	assert(glGetError() == GL_NO_ERROR);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat) * 3, _normals.data(), GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(3);
	assert(glGetError() == GL_NO_ERROR);

}

void ga_mesh::create_from_aiMesh(aiMesh* mesh, const aiScene* scene)
{
	GLenum err;
	
	// request vertex array object and vertex buffer objects
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(4, _vbo);

	get_vertices(mesh); // transfer the vertex attributes from the mesh

	ga_lit_material* mat = new ga_lit_material();
	mat->init();

	// TODO: Homework 4
	// set the diffuse color for the material 

	// get color from the scene->mMaterials[] according to the mesh->mMaterialIndex
	// check out the structure of ga_lit_material
	aiColor3D color (0.f,0.f,0.f);
	scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);
	mat->set_color({ color.r,color.g,color.b });

	_material = mat;

	make_buffers(); // setup the vertex buffers

	// unbind vertex array
	glBindVertexArray(0);

	err = glGetError();
	assert(err == GL_NO_ERROR);
}

void ga_mesh::assemble_drawcall(ga_static_drawcall& draw)
{
	draw._vao = _vao;
	draw._index_count = _index_count;
	draw._draw_mode = GL_TRIANGLES;
	draw._material = _material;
}



