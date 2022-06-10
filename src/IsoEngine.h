#pragma once

#include "CustomTypes.h"
#include "fmt/core.h"
#include <stdlib.h>

class IsoEngine {
	public:
	static void convert2DToIso(Point2DT* point);
	static void convertIsoTo2D(Point2DT* point);
};

