#pragma once
#include <fstream>

#include "IO/IOHelpers.hpp"
#include "Common/StringBuffer.hpp"
#include "Common/StringBuilder.hpp"
#include "Collections/List.hpp"

namespace Micro
{
	struct FileInfo final
	{
		String contents;
		String path;
		StringBuffer extension;

		constexpr FileInfo() noexcept = default;

		constexpr FileInfo(const String& path, String&& data) noexcept
			: contents(std::move(data)), path(path)
		{
			const auto& result = path.IndexOf('.');
			if (!result.IsValid()) 
				return;

			const StringBuffer bufferView = path.Data();
			extension = bufferView.Slice(result.Value()).Value();
		}
	};

	class FileHandler final
	{
	public:
		constexpr FileHandler() noexcept = delete;

		NODISCARD static bool Create(const char* path) noexcept
		{
			if (std::ofstream out(path, std::ios::out); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		NODISCARD static bool Create(const String& path) noexcept
		{
			if (std::ofstream out(path.Data(), std::ios::out); out)
			{
				out.close();
				return true;
			}

			return false;
		}

		NODISCARD static Result<FileInfo> GetFileInfo(const char* path) noexcept
		{
			if (std::ifstream in(path, std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				const StringBuilder contents{static_cast<u32>(in.tellg())};
				in.seekg(0, std::ios::beg);

				in.read(contents.ToCharArray(), static_cast<i64>(contents.Length()));
				in.close();

				return Result<FileInfo>::Ok({path, contents.ToString()});
			}

			return Result<FileInfo>::CaptureError(IOError(path));
		}

		NODISCARD static Result<FileInfo> GetFileInfo(const String& path) noexcept
		{
			if (std::ifstream in(path.Data(), std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				const StringBuilder contents{ static_cast<u32>(in.tellg()) };
				in.seekg(0, std::ios::beg);

				in.read(contents.ToCharArray(), static_cast<i64>(contents.Length()));
				in.close();

				return Result<FileInfo>::Ok({ path, contents.ToString() });
			}
			return Result<FileInfo>::CaptureError(IOError(path));
		}

		NODISCARD static Result<String> ReadAllText(const char* path) noexcept
		{
			if (std::ifstream in(path, std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents{ static_cast<u32>(in.tellg()) };
				in.seekg(0, std::ios::beg);

				while (!in.eof())
				{
					const char character = static_cast<char>(in.get());
					contents.Append(character);
				}

				in.close();
				contents.Remove(contents.Length() - 1);
				return Result<String>::Ok({ contents.ToString() });
			}

			return Result<String>::CaptureError(IOError(path));
		}

		NODISCARD static Result<String> ReadAllText(const String& path) noexcept
		{
			if (std::ifstream in(path.Data(), std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents{ static_cast<u32>(in.tellg()) };
				in.seekg(0, std::ios::beg);

				while (!in.eof())
				{
					const char character = static_cast<char>(in.get());
					contents.Append(character);
				}

				in.close();
				contents.Remove(contents.Length() - 1);
				return Result<String>::Ok({ contents.ToString() });
			}

			return Result<String>::CaptureError(IOError(path));
		}

		NODISCARD static Result<List<String>> ReadAllLines(const String& path) noexcept
		{
			if (std::ifstream in(path.Data(), std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents{ static_cast<u32>(in.tellg()) };
				in.seekg(0, std::ios::beg);

				List<String> fileLines(contents.Capacity());
				while (!in.eof())
				{
					const char character = static_cast<char>(in.get());
					if (character == '\n')
					{
						fileLines.Emplace(contents.ToString());

						contents.Clear();
						continue;
					}

					contents.Append(character);
				}

				in.close();
				contents.Remove(contents.Length() - 1);
				fileLines.Emplace(contents.ToString());
				return Result<List<String>>::Ok(fileLines);
			}
			return Result<List<String>>::CaptureError(IOError(path));
		}

		NODISCARD static Result<List<String>> ReadAllLines(const char* path) noexcept
		{
			if (std::ifstream in(path, std::ios::in); in)
			{
				in.seekg(0, std::ios::end);
				StringBuilder contents{ static_cast<u32>(in.tellg()) };
				in.seekg(0, std::ios::beg);

				List<String> fileLines(contents.Capacity());
				while (!in.eof())
				{
					const char character = static_cast<char>(in.get());
					if (character == '\n')
					{
						fileLines.Emplace(contents.ToString());

						contents.Clear();
						continue;
					}

					contents.Append(character);
				}

				in.close();
				contents.Remove(contents.Length() - 1);
				fileLines.Emplace(contents.ToString());
				return Result<List<String>>::Ok(fileLines);
			}
			return Result<List<String>>::CaptureError(IOError(path));
		}

		static void WriteAllText(const char* path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path, std::ios::out); out)
			{
				out << contents;
				out.close();
			}
		}

		static void WriteAllText(const String& path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path.Data(), std::ios::out); out)
			{
				out << contents;
				out.close();
			}
		}

		template <Streamable T>
		static void WriteAllLines(const char* path, const Span<T>& contents) noexcept
		{
			if (std::ofstream out(path, std::ios::out); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void WriteAllLines(const String& path, const Span<T>& contents) noexcept
		{
			if (std::ofstream out(path.Data(), std::ios::out); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		static void AppendAllText(const char* path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path, std::ios::app); out)
			{
				out << contents;
				out.close();
			}
		}

		static void AppendAllText(const String& path, const Streamable auto& contents) noexcept
		{
			if (std::ofstream out(path.Data(), std::ios::app); out)
			{
				out << contents;
				out.close();
			}
		}

		template <Streamable T>
		static void AppendAllLines(const char* path, const Span<T>& contents) noexcept
		{
			if (std::ofstream out(path, std::ios::app); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}

		template <Streamable T>
		static void AppendAllLines(const String& path, const Span<T>& contents) noexcept
		{
			if (std::ofstream out(path.Data(), std::ios::app); out)
			{
				for (const auto& line : contents)
					out << line << "\n";
				out.close();
			}
		}
	};
}
