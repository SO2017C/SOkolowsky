#pragma once
#include "Biblioteki.h"
#include "ACL.h"



struct FAT_struct {//structure for keeping info about file
	std::string file_name; //name of file
	int file_size; //size of file (without bits for indexes)
	int first_block_index; //index of firs data's block
};//end of FAT_struct

class HDD {
private:
	//GLOBALNE
	unsigned int const disc_size = 1024;
	unsigned int const block_size = 32;
	unsigned int block_number = disc_size / block_size;

	//DATA FIELDS
	std::array<char, 1024> data_conteiner;
	std::vector<FAT_struct> FAT_table;


	//BASIC FUNCTIONS
	bool check_file_exist(std::string file_to_save_name);
	bool check_space(int file_to_save_size);
	int check_first_empty_block();
	void uniforming_format_of_data_index(int index_index_block, int index_data_block, int counter_tmp);

	//EXTENSIONS
	void detailed_disk_view();
	void bit_vector_view();
	void fat_table_view();
	void percentage_disk_view();
	void percentage_sectors_view();

public:
	HDD();
	~HDD();
	void create_file(std::string file_to_save_name, int file_to_save_size, std::string file_to_save_text);
	void rename_file(std::string file_old_name, std::string file_new_name);
	void format_disk();
	std::string read_file(std::string file_to_read_name);
	void delete_file(std::string file_to_delete_name);
	 //EXTENSION
	void show_all_info();

};//class HDD