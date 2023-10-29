#pragma once
#include "gl_includes.h"
#include <string>
#include <vector>
#include <functional>
#include <Content.h>

using ContentTraversalVisitorT = void(const Content& content, int nodeId, const glm::mat4& mvp);
void traverseModel(const Content& content, std::function<ContentTraversalVisitorT>& visitor);

void traverseModelNode(const Content& content, int nodeId, std::vector<int> path, const glm::mat4& mvp, std::function<ContentTraversalVisitorT>& visitor);
