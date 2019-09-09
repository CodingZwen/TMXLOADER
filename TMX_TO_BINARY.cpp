#include "TMX_TO_BINARY.h"



TMX_TO_BINARY::TMX_TO_BINARY()
{
	stmx_file = "";
	debug = false;
	

}


TMX_TO_BINARY::~TMX_TO_BINARY()
{

}

void TMX_TO_BINARY::read_tmx_file(std::string & dateipfadtmx)
{
	stmx_file = "";
	std::ifstream Quelldatei;
	std::string datei = dateipfadtmx;
	int rows = 0;


	Quelldatei.open(datei, std::ios_base::in);

	//komplette tmx file wird in einen string gesteckt
	for (std::string s; Quelldatei.good();)
	{
		getline(Quelldatei, s);
		stmx_file += s;
		stmx_file += "\n";
		
		rows++;
	}


	if (debug) {
		if (Quelldatei.good())printf("laden tmx success\n");
		else printf("tmx laden nicht succes\n");

		std::cout << "string der versucht wurde zu öffnen\n" << dateipfadtmx << "\n";
		std::cout << "string der versucht wurde zu öffnen\n" << datei << "\n";

		printf("es wurden %d zeilen eingelesen\n",rows);
		std::cout << "das ist die tmx file:\n" << stmx_file << std::endl;

	}

	Quelldatei.close();
}



/*
sortiert den einzelnen tmx string
in einzelne string, die tags sind einzelne 
strings '>' und die arrays sind 
string mit dem end tag noch drin
anzahl = tags + arrays (x+4)
*/
bool TMX_TO_BINARY::sort_tmx_file()
{
	size_t end = stmx_file.size();
	std::string buffer = "";

	if (end < 1)return false;



	for (size_t t = 0; t < end; t++) {

		buffer += stmx_file.at(t);

		if (stmx_file.at(t) == '>') {
			vtmx_all_strings.push_back(buffer);

			if (debug)
			{
				std::cout << "\n\nstring::::\n" << buffer << std::endl;
			}

			buffer = "";
		}
	}

	return true;

}

bool TMX_TO_BINARY::put_digits_in_vectors()
{

	std::string buffer = "";
	std::string intbuffer = "";
	std::vector<int> v_buffer;
	bool ismaparray = false;
	int count = 0;

	for (std::vector<std::string>::const_iterator i = vtmx_all_strings.begin(); i != vtmx_all_strings.end(); ++i) {
		// process i

		//einzelner string wird rausgeholt, die arrays sind schon so aufgeteilt
		//das die zahlenmaps bereits in kompletten strings sind aber noch mit ,
		//und müssten einzeln rausgeholt werden und in einen int vector gesteckt
		//werden
		//wir wissen wenn der string ein komma hat, sind wir im array string
		//siehe z - 152
		buffer += *i;

		size_t s = buffer.size();
		for (size_t t = 0; t < s; t++) {

			if (buffer.at(t) == ',') { ismaparray = true; }

		}

		if (ismaparray) {
			for (size_t t = 0; t < s; t++) {

				if (isdigit(buffer.at(t))) {
					intbuffer += buffer.at(t);
				}

				if (buffer.at(t) == ',' || buffer.at(t) == '<') {

					int zahl = std::stoi(intbuffer);
					v_buffer.push_back(zahl);

					if (debug) {

						std::cout << zahl << ":";
					}


					intbuffer = "";
				}

			}

			vlayer_arrays.push_back(v_buffer);
			printf("array reingepackt\n");
			v_buffer.clear();
			ismaparray = false;
			count++;

		}

		buffer = "";

	}

	if (debug) {

		printf("strings nach zahlen gefiltert\nund einzeln in %d vectoren steckt\n", count);

	}

	return true;
}

int TMX_TO_BINARY::convert_vectors_in_binary_array(unsigned int *& bin_array)
{

	if (vlayer_arrays.size() < 4 || vlayer_arrays.size() > 4) {
		printf("zuviele arrays in den tmx files size: %d \n",vlayer_arrays.size());
		return 0;
	}

	std::vector<int> v1;
	std::vector<int> v2;
	std::vector<int> v3;
	std::vector<int> v4;


	v1 = vlayer_arrays.at(0);
	v2 = vlayer_arrays.at(1);
	v3 = vlayer_arrays.at(2);
	v4 = vlayer_arrays.at(3);

	int counter = 0;
	int size = v1.size();
	bin_array = new unsigned int[size];
	printf("%d speicher geholt\n", size);
	
	



	counter = 0;
	for (const auto &wert : v1) //code gespart mit gangsterschleife
	{
		bin_array[counter] = wert;
		counter++;
	}


	counter = 0;
	for (const auto &wert : v2) //deko array
	{
		if (wert != 0)
		{
			int binaer_buffer = wert;		//hol mir die 11 bits ab 11ter stelle aus meinem dekoarray
			binaer_buffer <<= 11;			//und schiebe diese 11 bits nach links
			bin_array[counter] |= binaer_buffer; //setzt das 12-22, bzw odern		
		}
		counter++; // seiteneffekt
	}



	counter = 0;
	for (int wert : v3) // collision array
	{

		if (wert != 0)
		{
			int test = 1;
			test <<= 22;
			bin_array[counter] |= test; //setzt das neunte bit
		}
		counter++;
	}


	counter = 0;
	std::vector<int>::const_iterator iter;

	for (iter = v4.begin(); iter != v4.end(); iter++) //animation 
	{
		if (*iter != 0) {

			int test = 1;
			test <<= 23;
			bin_array[counter] |= test; //setzt das neunte bit


		}
		counter++;
	}

	
	//array groesse = ergibt sich aus der groesse des vektors und diese ergibt sich aus der tmx datei^^
	return size;
}

int TMX_TO_BINARY::do_all(std::string & dateipfadtmx, unsigned  int *&bin_array)
{

	read_tmx_file(dateipfadtmx);

	bool success = false;
	int arraysize = 0;

	success = sort_tmx_file();
	success = put_digits_in_vectors();
	arraysize = convert_vectors_in_binary_array(bin_array);

	if (success) {
		printf("erfolgreich array geladen!\n");
	}else {
	
		printf("irgendentwas ist schief gelaufen\n");
	}

	return arraysize;
}



/*
int * TMX_TO_BINARY::get_binary_maparray()
{
	if (ptr_binary_map_array != NULL) {

		return ptr_binary_map_array;
	}
	else {
		printf("achtung NULL pointer zurückgegeben!\n");
		return nullptr;

	}
}

*/