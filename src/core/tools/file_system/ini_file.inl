#pragma once

// #include <Glog/glog>

#include "ini_file.hpp"

namespace xuzy::FileSystem {

template <typename T>
inline T IniFile::get(const std::string& p_key) {
  if constexpr (std::is_same<bool, T>::value) {
	if (!is_key_existing(p_key)) return false;

	return string_to_boolean(m_data_[p_key]);
  } else if constexpr (std::is_same<std::string, T>::value) {
	if (!is_key_existing(p_key)) return std::string("NULL");

	return m_data_[p_key];
  } else if constexpr (std::is_integral<T>::value) {
	if (!is_key_existing(p_key)) return static_cast<T>(0);

	return static_cast<T>(std::atoi(m_data_[p_key].c_str()));
  } else if constexpr (std::is_floating_point<T>::value) {
	if (!is_key_existing(p_key)) return static_cast<T>(0.0f);

	return static_cast<T>(std::atof(m_data_[p_key].c_str()));
  } else {
	LOG(ERROR) << "The given type must be : bool, integral, floating point "
				  "or string";
	return T();
  }
}

template <typename T>
inline T IniFile::get_or_default(const std::string& p_key, T p_default) {
  return is_key_existing(p_key) ? Get<T>(p_key) : p_default;
}

template <typename T>
inline bool IniFile::set(const std::string& p_key, const T& p_value) {
  if (is_key_existing(p_key)) {
	if constexpr (std::is_same<bool, T>::value) {
	  m_data_[p_key] = p_value ? "true" : "false";
	} else if constexpr (std::is_same<std::string, T>::value) {
	  m_data_[p_key] = p_value;
	} else if constexpr (std::is_integral<T>::value) {
	  m_data_[p_key] = std::to_string(p_value);
	} else if constexpr (std::is_floating_point<T>::value) {
	  m_data_[p_key] = std::to_string(p_value);
	} else {
	  LOG(ERROR) << "The given type must be : bool, integral, floating point "
					"or string";
	}

	return true;
  }

  return false;
}

template <typename T>
inline bool IniFile::add(const std::string& p_key, const T& p_value) {
  if (!is_key_existing(p_key)) {
	if constexpr (std::is_same<bool, T>::value) {
	  register_pair(p_key, p_value ? "true" : "false");
	} else if constexpr (std::is_same<std::string, T>::value) {
	  register_pair(p_key, p_value);
	} else if constexpr (std::is_integral<T>::value) {
	  register_pair(p_key, std::to_string(p_value));
	} else if constexpr (std::is_floating_point<T>::value) {
	  register_pair(p_key, std::to_string(p_value));
	} else {
	  LOG(ERROR) << "The given type must be : bool, integral, floating point "
					"or string";
	}

	return true;
  }

  return false;
}
}  // namespace xuzy::FileSystem