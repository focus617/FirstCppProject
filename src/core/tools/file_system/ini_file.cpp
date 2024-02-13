#include "ini_file.hpp"

#include <filesystem>
#include <fstream>

xuzy::FileSystem::IniFile::IniFile(const std::string& p_filePath)
    : m_filepath_(p_filePath) {
  load();
}

void xuzy::FileSystem::IniFile::reload() {
  remove_all();
  load();
}

bool xuzy::FileSystem::IniFile::remove(const std::string& p_key) {
  if (is_key_existing(p_key)) {
    m_data_.erase(p_key);
    return true;
  }

  return false;
}

void xuzy::FileSystem::IniFile::remove_all() { m_data_.clear(); }

bool xuzy::FileSystem::IniFile::is_key_existing(const std::string& p_key) const {
  return m_data_.find(p_key) != m_data_.end();
}

void xuzy::FileSystem::IniFile::register_pair(const std::string& p_key,
                                             const std::string& p_value) {
  register_pair(std::make_pair(p_key, p_value));
}

void xuzy::FileSystem::IniFile::register_pair(const AttributePair& p_pair) {
  m_data_.insert(p_pair);
}

std::vector<std::string> xuzy::FileSystem::IniFile::get_formatted_content()
    const {
  std::vector<std::string> result;

  for (const auto& [key, value] : m_data_) result.push_back(key + "=" + value);

  return result;
}

void xuzy::FileSystem::IniFile::load() {
  std::fstream iniFile;
  iniFile.open(m_filepath_);

  if (iniFile.is_open()) {
    std::string currentLine;

    while (std::getline(iniFile, currentLine)) {
      if (is_valid_line(currentLine)) {
        currentLine.erase(
            std::remove_if(currentLine.begin(), currentLine.end(), isspace),
            currentLine.end());
        register_pair(extract_key_and_value(currentLine));
      }
    }

    iniFile.close();
  }
}

void xuzy::FileSystem::IniFile::rewrite() const {
  std::ofstream outfile;
  outfile.open(m_filepath_, std::ios_base::trunc);

  if (outfile.is_open()) {
    for (const auto& [key, value] : m_data_)
      outfile << key << "=" << value << std::endl;
  }

  outfile.close();
}

std::pair<std::string, std::string>
xuzy::FileSystem::IniFile::extract_key_and_value(const std::string& p_line) const {
  std::string key;
  std::string value;

  std::string* currentBuffer = &key;

  for (auto& c : p_line) {
    if (c == '=')
      currentBuffer = &value;
    else
      currentBuffer->push_back(c);
  }

  return std::make_pair(key, value);
}

bool xuzy::FileSystem::IniFile::is_valid_line(
    const std::string& p_attributeLine) const {
  if (p_attributeLine.size() == 0) return false;

  if (p_attributeLine[0] == '#' || p_attributeLine[0] == ';' ||
      p_attributeLine[0] == '[')
    return false;

  if (std::count(p_attributeLine.begin(), p_attributeLine.end(), '=') != 1)
    return false;

  return true;
}

bool xuzy::FileSystem::IniFile::string_to_boolean(
    const std::string& p_value) const {
  return (p_value == "1" || p_value == "T" || p_value == "t" ||
          p_value == "True" || p_value == "true");
}