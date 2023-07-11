#pragma once
#include <fstream>

#include "IO/IOHelpers.hpp"
#include "Common/String.hpp"
#include "Common/StringBuffer.h"
#include "Common/StringBuilder.h"
#include "Collections/List.hpp"

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

		constexpr FileInfo() noexcept = default;

		FileInfo(const String& path, String&& data) noexcept
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

		NODISCARD static bool Create(const char* path) noexcept
		{
			if (std::ofstream out(path); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		NODISCARD static bool Create(const String& path) noexcept
		{
			if (std::ofstream out(path.Data()); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		NODISCARD static FileInfo GetFileInfo(const char* path)
		{
			if (std::ifstream in(path, static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				String contents{static_cast<size_t>(in.tellg())};
				in.seekg(0, std::ios::beg);
				in.read(contents.Data(), static_cast<int64_t>(contents.Length()));
				in.close();
				FileInfo fileInfo{path, std::move(contents)};
				return fileInfo;
			}
			throw IOException(path);
		}

		NODISCARD static FileInfo GetFileInfo(const String& path)
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
			throw IOException(path.Data());
		}

		NODISCARD static String ReadAllText(const char* path)
		{
			if (std::ifstream in(path, static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				String contents{static_cast<size_t>(in.tellg())};
				in.seekg(0, std::ios::beg);
				in.read(contents.Data(), static_cast<int64_t>(contents.Length()));
				in.close();
				return contents;
			}
			throw IOException(path);
		}

		NODISCARD static String ReadAllText(const String& path)
		{
			if (std::ifstream in(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents((in.tellg()));
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), contents.Capacity());
				in.close();

				contents.SyncSize();
				return contents.ToString();
			}
			throw IOException(path.Data());
		}

		// TODO: Wait for String/StringBuffer/StringBuilder overhaul
		NODISCARD static List<String> ReadAllLines(const String& path)
		{
			if (std::ifstream in(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents((in.tellg()));
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), contents.Capacity());
				in.close();
				contents.SyncSize();

				return {};
			}
			throw IOException(path.Data());
		}

		// TODO: Wait for String/StringBuffer/StringBuilder overhaul
		NODISCARD static List<String> ReadAllLines(const char* path)
		{
			if (std::ifstream in(path, static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents((in.tellg()));
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), contents.Capacity());
				in.close();
				contents.SyncSize();

				return {};
			}
			throw IOException(path);
		}

		static void WriteAllText(const char* path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void WriteAllText(const String& path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				out << contents;
				out.close();
			}
		}

		template <Streamable T>
		static void WriteAllLines(const char* path, const Sequence<T>& contents) noexcept
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void WriteAllLines(const String& path, const Sequence<T>& contents) noexcept
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		static void AppendAllText(const char* path, const Streamable auto& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		static void AppendAllText(const String& path, const Streamable auto& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				out << contents;
				out.close();
			}
		}

		template <Streamable T>
		static void AppendAllLines(const char* path, const Sequence<T>& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void AppendAllLines(const String& path, const Sequence<T>& contents)
		{
			if (std::ofstream out(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}
	};
}
