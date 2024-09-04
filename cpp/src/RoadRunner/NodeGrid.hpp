#pragma once

#include "PathNode.hpp"
#include "PathSegment.hpp"
#include "List.hpp"

#define maxNodes 32
#define maxSegs 32

#define TypeNode 1
#define TypeSegment 2

class NodeGrid{
	public:
  int recognitionId = -1;
	List<PathNode> nodes;
	List<PathSegment> segs;
	
	NodeGrid();
	~NodeGrid();
};
