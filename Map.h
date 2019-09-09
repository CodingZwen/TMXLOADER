#pragma once
#include <string>
#include "TMX_TO_BINARY.h"
#include <SFML\Graphics.hpp>
/*
Eventuell die klasse mit einem uniqe pointer versehen
std::unique_ptr<unsigned int []> (y*x);

*/
class Map
{


	unsigned int *ptr_binary_map;
	unsigned int _size;


	unsigned int map_width;
	unsigned int map_height;
	//asset pixlbreite
	unsigned int tileset_width;

	 sf::Texture *ptr_texture;
	int *kacke;

public:
	Map();
	~Map();

	//copy buffer löschen damit keine copy erzeugt wird die speicher doppelt freigibt
	//Map(Map const &) = delete;

	Map(Map const &other)
		: ptr_binary_map(new unsigned int[other._size]), _size(other._size)
	{
		for (unsigned int i = 0; i < _size; i++) {
			ptr_binary_map[i] = other.ptr_binary_map[i];
		}

		printf("copy konstruktor benutzt!!!!!\n");
		
	}

	 sf::Texture*  get_texture();

	bool initalize_map_from_tmx(std::string &tmxdatei, sf::Texture &_texture, int assetwidth, int x, int y);

	//rückgabewert in kacheln, also * 32 = pixel
	unsigned int get_width()const { return  this->map_width; }
	unsigned int get_height()const { return this->map_height; }
	unsigned int get_asset_width()const { return this->tileset_width; }

	unsigned int size() { return _size; }
	unsigned int &operator[](unsigned int position);
	Map &operator=(Map  &other);

	unsigned int *get_array_ptr() { return ptr_binary_map; }


	//return layers
	//return bitweise
	unsigned int first_layer(unsigned int position);
	unsigned int second_layer(unsigned int position);
	void change_deko_layer(unsigned int pos, unsigned int newfield = 0);


	bool animation_tile(unsigned int position);
	bool collision_tile(unsigned int position);


	//testzeugs

	int groesse() { return sizeof(*this); }

	
	
};

//schaut nach ob an 23er stelle 0 oder 1 ist
//bool returncoll_binary(int n) { return ptr_binary_map[n] >> 22; }
