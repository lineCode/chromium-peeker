/**
 * Part of WinLamb - Win32 API Lambda Library - More
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb-more
 */

#pragma once
#include "str.h"

namespace wl {

// Utilities to file path operations with std::wstring.
class path final {
protected:
	path() = default;

public:
	static std::wstring& trim_backslash(std::wstring& filePath) {
		while (filePath.back() == L'\\') {
			filePath.resize(filePath.length() - 1);
		}
		return filePath;
	}

	static bool is_same(const std::wstring& filePath, const std::wstring& other) {
		return !lstrcmpiW(filePath.c_str(), other.c_str());
	}

	static bool has_extension(const std::wstring& filePath, const wchar_t* extension) {
		if (extension[0] == L'.') {
			return str::ends_withi(filePath, extension);
		}

		wchar_t dotExtension[16] = { L'.', L'\0' };
		lstrcatW(dotExtension, extension);
		return str::ends_withi(filePath, dotExtension);
	}

	static bool has_extension(const std::wstring& filePath, const std::wstring& extension) {
		return has_extension(filePath, extension.c_str());
	}

	static bool has_extension(const std::wstring& filePath, std::initializer_list<const wchar_t*> extensions) {
		for (const wchar_t* ext : extensions) {
			if (has_extension(filePath, ext)) {
				return true;
			}
		}
		return false;
	}

	static std::wstring& change_extension(std::wstring& filePath, const wchar_t* newExtension) {
		size_t dotIdx = filePath.find_last_of(L'.');
		if (dotIdx != std::wstring::npos) {
			filePath.resize(dotIdx + 1); // truncate after the dot
		} else {
			filePath.append(1, L'.');
		}
		filePath.append(newExtension[0] == L'.' ? newExtension + 1 : newExtension);
		return filePath;
	}

	static std::wstring& change_extension(std::wstring& filePath, const std::wstring& newExtension) {
		return change_extension(filePath, newExtension.c_str());
	}

	static std::wstring folder_from(const std::wstring& filePath, bool withBackslash = false) {
		std::wstring ret(filePath);
		size_t found = ret.find_last_of(L'\\');
		if (found != std::wstring::npos) {
			ret.resize(found);
			if (withBackslash) ret += L'\\';
		}
		return ret;
	}

	static std::wstring file_from(const std::wstring& filePath) {
		std::wstring ret(filePath);
		size_t found = ret.find_last_of(L'\\');
		if (found != std::wstring::npos) {
			ret.erase(0, found + 1);
		}
		return ret;
	}
};

}//namespace wl