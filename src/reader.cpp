#include "reader.hpp"
#include <fstream>
#include <stdexcept>

void Reader::addSearchPath(std::string path)
{
	searchPaths_.push_back(path);
}

std::string Reader::read(std::string filename)
{
	if (std::filesystem::exists(filename) && !std::filesystem::is_directory(filename))
	{
		return std::move(internalRead(filename));
	}
	else
	{
		for (auto path : searchPaths_)
		{
			auto fullPath = path / filename;
			if (std::filesystem::exists(fullPath) && !std::filesystem::is_directory(fullPath))
				return std::move(internalRead(fullPath.string()));
		}
	}
	throw std::runtime_error("File \"" + filename + "\" not found!");
}

std::string Reader::internalRead(std::string path)
{
	auto fsize = std::filesystem::file_size(path);
	std::string readedFile;
	std::ifstream fstream(path, std::ios::binary | std::ios::in);
	if (!fstream.is_open())
	{
		throw std::runtime_error("Can't open \"" + path + "\"!");
	}
	readedFile.resize(fsize);
	while (!fstream.eof())
	{
		fstream.read(readedFile.data(), fsize);
	}
	return std::move(readedFile);
}
