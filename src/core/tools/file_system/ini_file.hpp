#pragma once

#include <string>
#include <unordered_map>

namespace xuzy::FileSystem {
/**
 * The IniFile class represents a file .ini that stores a set of
 * attributes/values that can get read and written
 */
class IniFile final {
 public:
  using AttributePair = std::pair<std::string, std::string>;
  using AttributeMap = std::unordered_map<std::string, std::string>;

  /**
   * @brief Create an IniFile by parsing the given file path and extracting
   * key/values pairs for future usage
   * @param p_filePath
   */
  IniFile(const std::string& p_filePath);

  /**
   * @brief Overwrite the content of the current data by reloading the file
   */
  void reload();

  /**
   * @brief Rewrite the entiere .ini file with the current values.
   * This operation is destructive and can't be undone. Any comment or line
   * break in your .ini file will get destroyed
   */
  void rewrite() const;

  /**
   * @brief Return the value attached to the given key
   * If the key doesn't exist, a default value is returned (0, false, "NULL")
   * @param p_key
   */
  template <typename T>
  T get(const std::string& p_key);

  /**
   * @brief Return the value attached to the given key
   * If the key doesn't exist, the specified value is returned
   * @param p_key
   * @param p_default
   */
  template <typename T>
  T get_or_default(const std::string& p_key, T p_default);

  /**
   * @brief Set a new value to the given key (Not applied to the real file
   * untill rewrite() or Save() is called)
   * @param p_key
   * @param p_value
   */
  template <typename T>
  bool set(const std::string& p_key, const T& p_value);

  /**
   * @brief Add a new key/value to the IniFile object (Not applied to the real
   * file untill rewrite() or Save() is called)
   * @param p_key
   * @param p_value
   */
  template <typename T>
  bool add(const std::string& p_key, const T& p_value);

  /**
   * @brief Remove an key/value pair identified by the given key (Not applied to
   * the real file untill rewrite() or Save() is called)
   * @param p_key
   */
  bool remove(const std::string& p_key);

  /**
   * @brief Remove all key/value pairs (Not applied to the real file untill
   * rewrite() or Save() is called)
   */
  void remove_all();

  /**
   * @brief Verify if the given key exists
   * @param p_key
   */
  bool is_key_existing(const std::string& p_key) const;

  /**
   * @brief Get the content stored in the ini file as a vector of strings (Each
   * string correspond to an attribute pair : Attribute=Value
   */
  std::vector<std::string> get_formatted_content() const;

 private:
  void register_pair(const std::string& p_key, const std::string& p_value);
  void register_pair(const AttributePair& p_pair);

  void load();

  AttributePair extract_key_and_value(const std::string& p_attributeLine) const;
  bool is_valid_line(const std::string& p_attributeLine) const;
  bool string_to_boolean(const std::string& p_value) const;

 private:
  std::string m_filepath_;
  AttributeMap m_data_;
};
}  // namespace xuzy::FileSystem

#include "ini_file.inl"