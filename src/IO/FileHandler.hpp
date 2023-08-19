#pragma once
#include <fstream>

#include "IO/IOHelpers.hpp"
#include "Common/String.hpp"
#include "Common/StringBuffer.hpp"
#include "Common/StringBuilder.hpp"
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
			const auto& result = path.IndexOf('.');
			if (!result.IsValid()) 
				return;

			const StringBuffer bufferView = path;
			extension = bufferView.Slice(result);
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
				const StringBuilder contents{static_cast<size_t>(in.tellg())};
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), static_cast<int64_t>(contents.Length()));
				in.close();
				FileInfo fileInfo{path, contents.ToString()};
				return fileInfo;
			}
			throw IOError(path);
		}

		NODISCARD static FileInfo GetFileInfo(const String& path)
		{
			if (std::ifstream in(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents(static_cast<size_t>(in.tellg()));
				in.seekg(0, std::ios::beg);
				in.read(contents.ToCharArray(), contents.Capacity());
				in.close();

				//contents.SyncSize();
				return {path, contents.ToString()};
			}
			throw IOError(path.Data());
		}

		NODISCARD static String ReadAllText(const char* path)
		{
			if (std::ifstream in(path, std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder builder(in.tellg());
				in.seekg(0, std::ios::beg);

				while (!in.eof())
				{
					const char character = in.get();
					builder.Append(character);
				}

				in.close();
				builder.Remove(builder.Length() - 1);
				return builder.ToString();
			}
			throw IOError(path);
		}

		NODISCARD static String ReadAllText(const String& path)
		{
			if (std::ifstream in(path.Data(), std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder builder(in.tellg());
				in.seekg(0, std::ios::beg);

				while (!in.eof())
				{
					const char character = in.get();
					builder.Append(character);
				}

				in.close();
				builder.Remove(builder.Length() - 1);
				return builder.ToString();
			}
			throw IOError(path.Data());
		}

		// TODO: Wait for String/StringBuffer/StringBuilder overhaul
		NODISCARD static List<String> ReadAllLines(const String& path)
		{
			if (std::ifstream in(path.Data(), static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder builder(in.tellg());
				in.seekg(0, std::ios::beg);

				List<String> fileLines(builder.Capacity());
				while (!in.eof())
				{
					const char character = in.get();
					if (character == '\n')
					{
						fileLines.Emplace(builder.ToString());

						builder.Clear();
						continue;
					}

					builder.Append(character);
				}

				in.close();
				builder.Remove(builder.Length() - 1);
				fileLines.Emplace(builder.ToString());
				return fileLines;
			}
			throw IOError(path.Data());
		}

		// TODO: Wait for String/StringBuffer/StringBuilder overhaul
		NODISCARD static List<String> ReadAllLines(const char* path)
		{
			if (std::ifstream in(path, static_cast<std::ios_base::openmode>(FileAccess::Read)); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder builder(in.tellg());
				in.seekg(0, std::ios::beg);

				List<String> fileLines(builder.Capacity());
				while (!in.eof())
				{
					const char character = in.get();
					if (character == '\n')
					{
						fileLines.Emplace(builder.ToString());

						builder.Clear();
						continue;
					}

					builder.Append(character);
				}

				in.close();
				builder.Remove(builder.Length() - 1);
				fileLines.Emplace(builder.ToString());
				return fileLines;
			}
			throw IOError(path);
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
		static void WriteAllLines(const char* path, const Span<T>& contents) noexcept
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Write)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void WriteAllLines(const String& path, const Span<T>& contents) noexcept
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
		static void AppendAllLines(const char* path, const Span<T>& contents)
		{
			if (std::ofstream out(path, static_cast<std::ios_base::openmode>(FileAccess::Append)); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void AppendAllLines(const String& path, const Span<T>& contents)
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
