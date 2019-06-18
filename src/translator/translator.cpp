#include "translator.hpp"
#include <stdexcept>
#include <spirv.hpp11>
#include <numeric>
#include <limits>

Translator::Translator() :
	humanReadable_(false)
{

}

void Translator::setHumanReadable(bool readable)
{
	humanReadable_ = readable;
}

std::variant<std::string, std::vector<uint32_t>> Translator::translate(std::vector<SpirVOp> ops)
{
	std::string resultStr;
	std::vector<uint32_t> resultWords;
	if (humanReadable_)
	{
		for (auto &op : ops)
			resultStr += translateHumanReadable(op);
		return resultStr;
	}
	else
	{
		resultWords.push_back(spv::MagicNumber);
		resultWords.push_back(spv::Version);
		resultWords.push_back(0); // magic number generator
		
		Id maxId = {};
		for (auto &op : ops)
		{
			for (auto &param : op.params)
			{
				if (param.type != OpParamType::Id)
					continue;
				if (param.id.id > maxId.id)
					maxId = param.id;
			}
		}
		resultWords.push_back(maxId.id + 1); // id bound
		resultWords.push_back(0); // reserved

		uint64_t currentWord = resultWords.size();

		for (auto &op : ops)
		{
			op.wordStart = currentWord;
			auto words = translateBinary(op);
			resultWords.insert(resultWords.end(), words.begin(), words.end());
			currentWord = resultWords.size();
			op.wordEnd = currentWord - 1;
		}
		return resultWords;
	}
}

std::string Translator::translateHumanReadable(const SpirVOp &op)
{
	return std::string();
}

std::vector<uint32_t> Translator::translateBinary(const SpirVOp &op)
{
	uint16_t opCode = static_cast<uint16_t>(op.op);
	std::vector<uint32_t> words;
	for (auto &param : op.params)
	{
		switch (param.type)
		{
		case OpParamType::Id:
			words.push_back(param.id.id);
			break;
		case OpParamType::LiteralI:
			switch (param.numSize)
			{
			case 32:
			{
				auto num = static_cast<int32_t>(param.inum);
				words.push_back(*reinterpret_cast<uint32_t*>(&num));
				break;
			}
			case 64:
			{
				auto num = static_cast<int64_t>(param.inum);
				auto ptr = reinterpret_cast<uint32_t*>(&num);
				words.push_back(*ptr++);
				words.push_back(*ptr);
				break;
			}
			default:
				throw std::runtime_error("Unsupported number size");
			}
			break;
		case OpParamType::LiteralU:
			switch (param.numSize)
			{
			case 32:
			{
				auto num = static_cast<uint32_t>(param.inum);
				words.push_back(*reinterpret_cast<uint32_t*>(&num));
				break;
			}
			case 64:
			{
				auto num = static_cast<uint64_t>(param.unum);
				auto ptr = reinterpret_cast<uint32_t*>(&num);
				words.push_back(*ptr++);
				words.push_back(*ptr);
				break;
			}
			default:
				throw std::runtime_error("Unsupported number size");
			}
			break;
		case OpParamType::Float:
			static_assert(std::numeric_limits<float>::is_iec559, "Unsupported compiler! Compiler must support IEEE 754 floating points.");
			static_assert(sizeof(float) == 4);
			static_assert(sizeof(double) == 8);
			switch (param.numSize)
			{
			case 32:
			{
				auto num = static_cast<float>(param.dnum);
				words.push_back(*reinterpret_cast<uint32_t*>(&num));
				break;
			}
			case 64:
			{
				auto num = static_cast<double>(param.dnum);
				auto ptr = reinterpret_cast<uint32_t*>(&num);
				words.push_back(*ptr++);
				words.push_back(*ptr);
				break;
			}
			default:
				throw std::runtime_error("Unsupported number size");
			}
			break;
		case OpParamType::String:
		{
			auto str = param.str;
			auto rem = (str.size()+1) % 4;
			if (rem != 0)
			{
				for (; rem != 4; rem++)
					str += '\0';
			}
			auto toWrite = (str.size()+1) / 4;
			const char *data = str.data();
			const uint32_t *rdata = reinterpret_cast<const uint32_t*>(data);
			do
			{
				words.push_back(*rdata);
				rdata++;
				toWrite--;
			} while (toWrite);
			break;
		}
		default:
			break;
		}
	}
	auto wordCount = static_cast<uint16_t>(words.size() + 1);
	words.insert(words.begin(), makeFirstWord(wordCount, opCode));
	return std::move(words);
}


uint32_t Translator::makeFirstWord(uint16_t wordCount, uint16_t opCode)
{
	return ((opCode & spv::OpCodeMask) | (static_cast<uint32_t>(wordCount) << spv::WordCountShift));
}