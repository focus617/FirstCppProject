#pragma once

#include <string>

#include "core.h"

namespace xuzy::Tools::FileSystem
{
	/**
	* @brief Provide some tools to get information about a given path
	*/
	class XUZY_API PathParser
	{
	public:
		enum class EFileType
		{
			UNKNOWN,
			MODEL,
			TEXTURE,
			SHADER,
			MATERIAL,
			SOUND,
			SCENE,
			SCRIPT,
			FONT
		};

		/**
		* @brief Disabled constructor
		*/
		PathParser() = delete;

		/**
		* @brief Returns the windows style version of the given path
		* ('/' replaced by '\')
		* @param p_path
		*/
		static std::string make_windows_style(const std::string& p_path);

		/**
		* @brief Returns the non-windows style version of the given path
		* ('\' replaced by '/')
		* @param p_path
		*/
		static std::string make_non_windows_style(const std::string& p_path);

		/**
		* @brief Returns the containing folder of the file or folder 
		* identified by the given path
		* @param p_path
		*/
		static std::string get_containing_folder(const std::string& p_path);

		/**
		* @brief Returns the name of the file or folder identified by the
		* given path
		* @param p_path
		*/
		static std::string get_element_name(const std::string& p_path);

		/**
		* @brief Returns the extension of the file or folder identified by
		* the given path
		* @param p_path
		*/
		static std::string get_extension(const std::string& p_path);

		/**
		* @brief Convert the EFileType value to a string
		* @param p_fileType
		*/
		static std::string file_type_to_string(EFileType p_fileType);

		/**
		* @brief Returns the file type of the file identified by the given path
		* @param p_path
		*/
		static EFileType get_file_type(const std::string& p_path);
	};

} // namespace xuzy::Tools::FileSystem