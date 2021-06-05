#pragma once

#include <ostream>
#include <iostream>
#include <string>

class HeaderFile;

std::string get_namespace_out_of_filename(const std::string& filename);
void serialize(std::ostream& out, const HeaderFile& header);
