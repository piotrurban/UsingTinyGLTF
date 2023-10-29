#include "ContentUtils.h"
#include <TransformUtils.h>
#include <TraverseContent.h>
#include <utils.h>
#include <map>
#include <cmath>

using namespace std::placeholders;

float nearestZRayCast(float x, float y, const Content& content, int nodeId)
{
	const int meshId = content.m_model.nodes.at(nodeId).mesh;
	const std::vector<glm::vec3>& positions = getMeshToPositionsMap(content).at(meshId);
	assert(positions.size() % 3 == 0);
	const glm::mat4& mvp = glm::mat4(getContentMVP(content)) * getNodeToMVPMap(content).at(nodeId);
	float nearestZ = std::numeric_limits<float>::max();
	int count = 0;
	int nearestTriangleId = -1;
	for (auto it = positions.cbegin(); it != positions.cend(); )
	{
		const glm::vec4 v1{ mvp * glm::vec4(*it, 1.0) };
		const glm::vec4 v2{ mvp * glm::vec4(*(it + 1), 1.0) };
		const glm::vec4 v3{ mvp * glm::vec4(*(it + 2), 1.0) };
		std::array<glm::vec3, 3> triangle{ glm::vec3(v1 / v1.w), glm::vec3(v2 / v2.w), glm::vec3(v3 / v3.w) };
		if (isTriangleInViewport(triangle))
		{
			std::cout << std::format("nearestZRayCast: triangle #{}: {}, {}, {}\n", count, triangle.at(0), triangle.at(1), triangle.at(2));
		}
		float z = triangleZRayCast(triangle, glm::vec2(x, y)).z;
		if (z < nearestZ)
		{
			nearestTriangleId = count;
			nearestZ = z;
		}
		it += 3;
		count++;
	}
	std::cout << "nearest ID: " << nearestTriangleId << std::endl;
	return nearestZ;
}

void ComputeNodeMVPVisitor::operator()(const Content& content, int nodeId, const glm::mat4& mvp)
{
	std::cout << "calling CollectVisitor for node " << nodeId << "\n";
	m_nodeToMVPMap[nodeId] = mvp;
}

const std::map<int, glm::mat4>& getNodeToMVPMap(const Content& content)
{
	using NodeToMVPMapT = std::map<int, glm::mat4>;
	static std::map<size_t, NodeToMVPMapT> contentToNodeMap{};
	if (!contentToNodeMap.contains((size_t)(&content)))
	{
		ComputeNodeMVPVisitor collect{};
		std::function<ContentTraversalVisitorT> computeMVPsVisitor = std::bind(&ComputeNodeMVPVisitor::operator(), &collect, _1, _2, _3);
		traverseModel(content, computeMVPsVisitor);
		contentToNodeMap[(size_t)(&content)] = collect.m_nodeToMVPMap;
	}
	return contentToNodeMap.at((size_t)(&content));
}
