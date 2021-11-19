#pragma once

#include "CustomTypes.h"

class IsoEngine {
	public:
	IsoEngine();
	~IsoEngine();

	void convert2DToIso(Point2DT* point);
};

