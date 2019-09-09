#include "Map.h"



Map::Map()
{
	

	ptr_binary_map = NULL;
	_size = 0;
	printf("Map erstellt!!\n\n");
	
}


Map::~Map()
{
	if(ptr_binary_map != NULL)delete[] ptr_binary_map;
}

 sf::Texture*  Map::get_texture()
{
	return ptr_texture;
}

bool Map::initalize_map_from_tmx(std::string & tmxdatei, sf::Texture &_texture, int assetwidth, int x, int y)
{

	TMX_TO_BINARY tmxreader;
	
	ptr_texture = &_texture;

	

	//auf grund des viewports mit float zahlen gibt es schwarze linien, glaube ich zumindest?
	//_texture.setSmooth(true);

	//tmx funktion holt speicher für das array :P
	//size = anzahl
     _size=tmxreader.do_all(tmxdatei, ptr_binary_map);

	 printf("size: %d   x: %d y: %d x*y: %d\n", _size, x, y, x*y);

	map_width = x;
	map_height = y;
	tileset_width = assetwidth;
	
	if (_size == (x*y))return true;

	return false;
}




unsigned int & Map::operator[](unsigned int position)
{
	// TODO: hier Rückgabeanweisung eingeben

	if (position < _size) { return ptr_binary_map[position]; }

	//throw std::runtime_error("maparray outs of bounds mein sohn!!!\n");
	
}

Map & Map::operator=(Map  & other)
{


	delete[] ptr_binary_map;


	ptr_binary_map = new unsigned int[other._size];
	_size = other._size;

	for (unsigned int i = 0; i < _size; i++) {
		ptr_binary_map[i] = other.ptr_binary_map[i];
	}

	ptr_texture = other.get_texture();
	map_height = other.get_height();
	map_width = other.get_width();
	tileset_width = other.get_asset_width();

	
	return *this;

}

//get first 11 bits (2047=11111111111)
unsigned int Map::first_layer(unsigned int position)
{
	return (ptr_binary_map[position] & 2047);
}

unsigned int Map::second_layer(unsigned int position)
{
	//schiebt den mittlerenteil nach rechts
	unsigned int buffer = ptr_binary_map[position] >> 11;

	//und schneidet dann rest ab, sonst hängt col oder anim bit noch drin
	return buffer & 2047;

}

void Map::change_deko_layer(unsigned int position, unsigned int newfield)
{
	//4290775039 = 11111111110000000000011111111111
	unsigned int bitmask = 4290775039;
	unsigned int buffer = ptr_binary_map[position];
	unsigned int result = bitmask & buffer;
	ptr_binary_map[position] = result;

}

bool Map::animation_tile(unsigned int position)
{
	if (position > _size)
	{
		printf("achtung array in animationtile map\n");
		printf("funktion überrtreten!!!\n");
		return false;
	}

	if (ptr_binary_map[position] >> 23 != 0) {
		return true;
	}else {
		return false;
	}


}

bool Map::collision_tile(unsigned int position)
{
	if (position > _size)
	{
		printf("achtung array in animationtile map\n");
		printf("funktion überrtreten!!!\npossition: %d\n",position);
		return false;
	}


	return ptr_binary_map[position] >> 22&1;

}

