#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

void export_vec_to_file(std::vector<float> vec, std::string file_name);

void export_data(std::vector<std::vector<float>>& data, std::string file_name);

