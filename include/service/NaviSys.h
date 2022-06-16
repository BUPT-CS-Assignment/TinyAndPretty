#pragma once

#include <common.h>
#include <interfaces.h>

#include "../data/maps/navi_info.h"

#include "../data/maps/shahe_walking.h"
#include "../data/maps/shahe_bike.h"
#include "../data/maps/shahe_walking_busy.h"

#include "../data/maps/xtc_walking.h"
#include "../data/maps/xtc_bike.h"
#include "../data/maps/xtc_walking_busy.h"

#define CALC_UNITS(arr)  ( sizeof(arr)    / sizeof(arr[0][0]) )
#define CALC_WIDTH(arr)  ( sizeof(arr[0]) / sizeof(arr[0][0]) ) 
#define CALC_HEIGHT(arr) ( CALC_UNITS(arr) / CALC_WIDTH(arr)  )

constexpr int SHAHE_MAX_HEIGHT = CALC_HEIGHT(shahe_walking);
constexpr int SHAHE_MAX_WIDTH  = CALC_WIDTH(shahe_walking);

constexpr int XTC_MAX_HEIGHT = CALC_HEIGHT(xtc_walking);
constexpr int XTC_MAX_WIDTH  = CALC_WIDTH(xtc_walking);

#define WALKABLE   0
#define INF 0xfffffff

static constexpr int dx[] = { 0, 0,-1, 1};
static constexpr int dy[] = {-1, 1, 0, 0};

constexpr size_t arrLen = (sizeof(dx) / sizeof(int));

struct Point2D {
	int x 	= 0; 
	int y 	= 0;
	int dis = INF;
	Point2D* parent = nullptr;
};

enum EvalueMode {
	SHORT_PATH ,
	SHORT_TIME ,
	SHORT_BIKE
};

enum MapValue {
	SH,
	XTC
};