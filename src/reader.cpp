#include "reader.hpp"
#include <fstream>
#include <stdexcept>

void Reader::addSearchPath(std::string path)
{
	searchPaths_.push_back(path);
}

std::string Reader::read(std::filesystem::path filename)
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
	throw std::runtime_error("File \"" + filename.string() + "\" not found!");
}

std::filesystem::path Reader::search(std::filesystem::path currentFolder, std::string filename, bool local)
{
	if (local)
	{
		auto localFile = currentFolder / filename;
		if (std::filesystem::exists(localFile) && !std::filesystem::is_directory(localFile))
			return localFile;
		throw std::runtime_error("File \"" + localFile.string() + "\" didn't exists!");
	}
	else
	{
		for (const auto &searchPath : searchPaths_)
		{
			auto globalFile = searchPath / filename;
			if (std::filesystem::exists(globalFile) && !std::filesystem::is_directory(globalFile))
				return globalFile;
		}
		throw std::runtime_error("Unable to find file \"" + filename + "\"!");
	}
}

std::string Reader::internalRead(std::filesystem::path path)
{
	auto fsize = std::filesystem::file_size(path);
	std::string readedFile;
	std::ifstream fstream(path, std::ios::binary | std::ios::in);
	if (!fstream.is_open())
	{
		throw std::runtime_error("Can't open \"" + path.string() + "\"!");
	}
	readedFile.resize(static_cast<size_t>(fsize));
	if (!fsize)
		return readedFile;
	while (!fstream.eof())
	{
		fstream.read(readedFile.data(), fsize);
	}
	return std::move(readedFile);
}
