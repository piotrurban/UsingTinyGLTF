#pragma once
#include <Content.h>
#include <utils.h>

using ContentTraversalVisitorT = void(const Content& content, int nodeId, const glm::mat4& mvp);

class ComputeNodeMVPVisitor
{
public:
	ContentTraversalVisitorT operator();

	std::map<int, glm::mat4> m_nodeToMVPMap;
};

float nearestZRayCast(float x, float y, const Content& content, int meshId);

const std::map<int, glm::mat4>& getNodeToMVPMap(const Content& content);