#pragma once
//-----------------------------------------------
#include <string_view>
//-----------------------------------------------
std::string_view readUntilAndCutWithTrim(std::string_view & str, char delim);

void trimLeft(std::string_view & str);
void trimRight(std::string_view & str);
void trim(std::string_view & str);
//-----------------------------------------------
