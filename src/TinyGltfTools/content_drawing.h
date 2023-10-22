#pragma once
#include "Content.h"

void draw_model(tinygltf::Model& model);
void draw_node(Content& model, const tinygltf::Node& node);
void draw_mesh(Content& content, tinygltf::Mesh& mesh);