#include "utils.h"


std::vector<std::string> split(const std::string& text, char separator)
{
	std::vector<std::string> result;
	size_t start = 0, end = 0;
	
	while((end = text.find(separator, start)) != std::string::npos)
	{
		result.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	
	result.push_back(text.substr(start));
	return result;
}

