#pragma once
#include <fstream>

#include "Common/String.h"

namespace mtk {
	
	struct FileInfo final
	{
		String contents;
		String path;
		BufferView extension;

		FileInfo(const String& _path, const String& _contents)
			: contents(_contents), path(_path)
		{
			const int32_t index = _path.IndexOf('.');
			if (index == -1) return;

			const BufferView bufferView = _path;
			extension = bufferView.Slice(index);
		}

	};

#define FILE_APPEND		8
#define FILE_WRITE		16
#define FILE_READ		2
	
	class FileHandler final
	{
	public:
		FileHandler() = delete;

		static bool Create(const char* _path)
		{
			if (std::ofstream out(_path); out) {
				out.close();
				return true;
			}

			return false;
		}

		static bool Create(const String& _path)
		{
			if (std::ofstream out(_path.Data()); out) {
				out.close();
				return true;
			}

			return false;
		}
		
		static FileInfo Read(const char* _path)
		{
			if (std::ifstream in(_path, FILE_READ); in) {
				in.seekg(0, std::ios::end);
				String contents { (size_t) in.tellg() };
				in.seekg(0, std::ios::beg);
				in.read((char*) contents.Data(), (int64_t) contents.Size());
				in.close();
				return { _path, contents };
			}
			throw errno;
		}
		
		static FileInfo Read(const String& _path)
		{
			if (std::ifstream in(_path.Data(), FILE_READ); in) {
				in.seekg(0, std::ios::end);
				String contents { (size_t) in.tellg() };
				in.seekg(0, std::ios::beg);
				in.read((char*) contents.Data(), (int64_t) contents.Size());
				in.close();
				return { _path, contents };
			}
			throw errno;
		}

		static void Write(const char* _path, const char* _contents)
		{
			if (std::ofstream out(_path, FILE_WRITE); out) {
				out << _contents;
				out.close();
			}
		}

		static void Write(const char* _path, const String& _contents)
		{
			if (std::ofstream out(_path, FILE_WRITE); out) {
				out << _contents;
				out.close();
			}
		}

		static void Write(const String& _path, const char* _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_WRITE); out) {
				out << _contents;
				out.close();
			}
		}

		static void Write(const String& _path, const String& _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_WRITE); out) {
				out << _contents;
				out.close();
			}
		}

		static void WriteLines(const char* _path, const Sequence<String>& _contents)
		{
			if (std::ofstream out(_path, FILE_WRITE); out) {
				for (const String& line : _contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void WriteLines(const String& _path, const Sequence<String>& _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_WRITE); out) {
				for (const String& line : _contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void Append(const char* _path, const char* _contents)
		{
			if (std::ofstream out(_path, FILE_APPEND); out) {
				out << _contents;
				out.close();
			}
		}

		static void Append(const char* _path, const String& _contents)
		{
			if (std::ofstream out(_path, FILE_APPEND); out) {
				out << _contents;
				out.close();
			}
		}

		static void Append(const String& _path, const char* _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_APPEND); out) {
				out << _contents;
				out.close();
			}
		}

		static void Append(const String& _path, const String& _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_APPEND); out) {
				out << _contents;
				out.close();
			}
		}

		static void AppendLines(const char* _path, const Sequence<String>& _contents)
		{
			if (std::ofstream out(_path, FILE_APPEND); out) {
				for (const String& line : _contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void AppendLines(const String& _path, const Sequence<String>& _contents)
		{
			if (std::ofstream out(_path.Data(), FILE_APPEND); out) {
				for (const String& line : _contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}
	};

}
