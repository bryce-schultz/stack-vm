#pragma once

#include <vector>

#include "IVisitor.h"

class Node
{
public:
	virtual ~Node()
	{
		for (auto child : children)
		{
			delete child;
		}
		children.clear();
	}

	void addChild(Node *child)
	{
		children.push_back(child);
	}

	Node *getChild(int index) const
	{
		return children[index];
	}

	virtual void visit(IVisitor *visitor) = 0;

	void visitAllChildren(IVisitor *visitor)
	{
		for (auto child : children)
		{
			child->visit(visitor);
		}
	}

private:
	std::vector<Node *> children;
};
