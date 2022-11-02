#pragma once
#include <string>
#include <fstream>

namespace mtk {

	struct FileData
	{
		std::string _contents;
		std::string _path;
		std::string_view _extension;

		FileData(std::string_view _path, const std::string& _contents)
			: _path(_path), _contents(_contents)
		{
			auto stringEnd = _path.substr(_path.length() - 5, 5);
			size_t loc = 0;
			for (auto i = 0; i < stringEnd.length(); i++) {
				if (stringEnd[i] == '.') {
					loc = i;
					break;
				}
			}
			_extension = stringEnd.substr(loc, stringEnd.length() - loc);
		}

	};

	class FileHandler
	{
	public:

		static FileData ReadFromFile(const char* _path)
		{
			std::ifstream in(_path, std::ios::binary);
			if (in) {
				std::string contents;
				in.seekg(0, std::ios::end);
				contents.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();
				return { _path, contents };
			}
			throw(errno);
		}

		static void WriteToFile(const char* _path, const char* _contents, const std::string& _opt = "a")
		{
			int openMode = 0;
			if (_opt == "a")
				openMode = std::ios::app;
			else if (_opt == "r")
				openMode = std::ios::fixed;
			else if (_opt == "w")
				openMode = std::ios::out;

			std::ofstream out(_path, openMode);
			if (out) {
				out << _contents;
				out.close();
			}
		}

		static void WriteToFile(const char* _path, const std::string& _contents, const std::string& _opt = "a")
		{
			int openMode = 0;
			if (_opt == "a")
				openMode = std::ios::app;
			else if (_opt == "r")
				openMode = std::ios::fixed;
			else if (_opt == "w")
				openMode = std::ios::out;

			std::ofstream out(_path, openMode);
			if (out) {
				out << _contents;
				out.close();
			}
		}


	private:
		FileHandler() = delete;
	};

}