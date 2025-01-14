#pragma once

#include <vector>

#include "IVisitor.h"

class Node
{
public:
	void addChild(Node *child)
	{
		children.push_back(child);
	}

	Node* getChild(int index) const
	{
		return children[index];
	}

	//*************************************
	// Used by visitor pattern. Every node must override with:
	//    virtual void visit(IVisitor *visitor) { visitor->visit(this); }
	virtual void visit(IVisitor *visitor) = 0;

	void visitAllChildren(IVisitor* visitor)
	{
		for (auto child : children)
		{
			child->visit(visitor);
		}
	}
private:
	std::vector<Node *> children;
};

