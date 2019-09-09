#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>


class TMX_TO_BINARY
{
public:
	TMX_TO_BINARY();
	~TMX_TO_BINARY();

	void read_tmx_file(std::string &dateipfadtmx);
	bool sort_tmx_file();
	bool put_digits_in_vectors();//sortiert die Tags und Kommas aus den Tmx string Arrays heraus //und sortiert sie in 4 int Vektoren. 
	int  convert_vectors_in_binary_array(unsigned int *&bin_array); //für das array wurd auch speicher geholt, groesse wird returned.

	//die 4 oberen funktionen zusammengefasst, arraygroesse wird returned, aber x und y fehlt sozusagen
	int do_all(std::string &dateipfadtmx, unsigned int *&bin_array);

	void _debug() { debug = true; }

	//int *get_binary_maparray();


		
private:

	bool debug;

	std::string stmx_file;
	std::vector<std::string> vtmx_all_strings;
	std::vector<std::vector<int>> vlayer_arrays;

};

