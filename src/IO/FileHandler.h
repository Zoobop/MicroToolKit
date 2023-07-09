#pragma once
#include <fstream>

#include "Common/String.h"
#include "Common/StringBuffer.h"
#include "Common/StringBuilder.h"

namespace Micro
{
	enum struct FileAccess : uint8_t
	{
		Read = 2,
		Append = 8,
		Write = 16
	};

	struct FileInfo final
	{
		String contents;
		String path;
		StringBuffer extension;

		FileInfo() = default;

		FileInfo(const String& path, String&& data)
			: contents(std::move(data)), path(path)
		{
			const int32_t index = path.IndexOf('.');
			if (index == -1) return;

			const StringBuffer bufferView = path;
			extension = bufferView.Slice(index);
		}
	};

	class FileHandler final
	{
	public:
		FileHandler() = delete;

		static bool Create(const char* path)
		{
			if (std::ofstream out(path); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		static bool Create(const String& path)
		{
			if (std::ofstream out(path.Data()); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		static FileInfo Read(const char* path)
		{
			if (std::ifstream in(path, static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				String contents{static_cast<size_t>(in.tellg())};
				in.seekg(0, std::ios::beg);
				in.read(const_cast<char*>(contents.Data()), static_cast<int64_t>(contents.Capacity()));
				in.close();
				FileInfo fileInfo{path, std::move(contents)};
				return fileInfo;
			}
			throw errno;
		}

		static FileInfo Read(const String& path)
		{
			if (std::ifstream in(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents((in.tellg()));
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), contents.Capacity());
				in.close();

				contents.SyncSize();
				return {path, contents.ToString()};
			}
			throw errno;
		}

		static void Write(const char* path, const char* contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Write(const char* path, const String& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Write(const String& path, const char* contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Write(const String& path, const String& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void WriteLines(const char* path, const Sequence<String>& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				for (const String& line : contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void WriteLines(const String& path, const Sequence<String>& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				for (const String& line : contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void Append(const char* path, const char* contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Append(const char* path, const String& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Append(const String& path, const char* contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void Append(const String& path, const String& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void AppendLines(const char* path, const Sequence<String>& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				for (const String& line : contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}

		static void AppendLines(const String& path, const Sequence<String>& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				for (const String& line : contents)
				{
					out << line << "\n";
				}
				out.close();
			}
		}
	};
}
