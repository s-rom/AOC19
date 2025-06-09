#include "../intcode/intcode.h"
#include "../util/util.h"
#include "../util/vec.h"

#include <raylib.h>



enum class Tile
{
	EMPTY = 0,
	WALL,
	BLOCK,
	PADDLE,
	BALL
};



void day13();
