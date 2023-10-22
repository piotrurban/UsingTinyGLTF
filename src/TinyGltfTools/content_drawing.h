#pragma once
#include "Content.h"

void draw_model(Content& model);
void draw_node(Content& model, const tinygltf::Node& node);
void draw_mesh(Content& content, tinygltf::Mesh& mesh);