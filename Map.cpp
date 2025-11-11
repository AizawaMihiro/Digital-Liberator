#include "Map.h"

/// <summary>
/// ステージである迷路を生成するクラス
/// 与える因数によって生成方法を変更する
/// </summary>

Map::Map()
	:Map(STICK)
{
}

Map::Map(generator gen)
{
	switch (gen)
	{
	case STICK:
		break;
	case DIG:
		break;
	case WALL:
		break;
	default:
		break;
	}
}

Map::~Map()
{
}

void Map::Instantinate()
{
}
