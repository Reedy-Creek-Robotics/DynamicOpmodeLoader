#pragma once
#include "JFunc.hpp"
#include "NodeGrid.hpp"

#include <string>

void print(const char* msg);
void err(const char* msg);

class Save
{
  public:
	static JFunc<void, jdouble, jdouble, jdouble> makeBuilder;
	static JFunc<void, jdouble, jdouble> lineTo;
	static JFunc<void, jdouble, jdouble, jdouble> splineTo;
	static JFunc<void, jdouble, jdouble, jdouble> lineToLinearHeading;
	static JFunc<void, jdouble, jdouble, jdouble, jdouble> splineToLinearHeading;
	static JFunc<void, jdouble, jdouble> lineToConstantHeading;
	static JFunc<void, jdouble, jdouble, jdouble> splineToConstantHeading;
	static JFunc<void, jdouble, jdouble, jdouble> lineToSplineHeading;
	static JFunc<void, jdouble, jdouble, jdouble, jdouble> splineToSplineHeading;
	static JFunc<void, jdouble> wait;
	static JFunc<void, jdouble> rotate;
	static JFunc<void, jstring> marker;
  static JFunc<void, jstring> pathErr;
	static bool load(NodeGrid* grid, const std::string& path);
	static bool exp(NodeGrid* grid);
};
