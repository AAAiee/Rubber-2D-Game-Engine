#pragma once

#include <string>
namespace Rubber {

	class fileDialog 
	{
	public:

		// these return empty string if cancelled
		static std::wstring openFile(const wchar_t* filter); 
		static std::wstring saveFile(const wchar_t* filter);
	};
	



}
