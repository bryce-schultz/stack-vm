//***********************************************
// Node.h
//***********************************************

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

	void setChild(size_t index, Node *child)
	{
		if (index >= children.size())
		{
			addChild(child);
			return;
		}

		delete children[index];
		children[index] = child;
	}

	Node *getChild(size_t index) const
	{
		return children[index];
	}

	int getChildCount() const
	{
		return children.size();
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
