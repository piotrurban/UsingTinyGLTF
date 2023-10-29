#include "TraverseContent.h"
#include "utils.h"
#include <iostream>

void traverseModel(const Content& content, std::function<ContentTraversalVisitorT>& visitor)
{
	std::vector<int> nodePath{};
	glm::mat4 mvp = glm::mat4(1.0);
	for (int nodeId = 0; nodeId < content.m_model.nodes.size(); nodeId++)
	{
		traverseModelNode(content, nodeId, nodePath, mvp, visitor);
	}
}

void traverseModelNode(const Content& content, int nodeId, std::vector<int> path, const glm::mat4& mvp, std::function<ContentTraversalVisitorT>& visitor)
{
	path.push_back(nodeId);
	const tinygltf::Node& node = content.m_model.nodes.at(nodeId);
	std::string nodeName{ node.name };
	std::cout << "traverseModelNode: " <<  nodeName << "\n";
	const glm::mat4 globalNodeMVP{mvp * getNodeMVP(content, nodeId)};
	visitor(content, nodeId, globalNodeMVP);

	for (int id = 0; id < node.children.size(); id++)
	{
		traverseModelNode(content, id, path, globalNodeMVP, visitor);
	}
}