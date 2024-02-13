#pragma once

#include "visibility_control.hpp"

namespace xuzy::Filesystem {
/**
 * @brief Class to manage external package.
 */
class XUZY_API Package {
 public:
  /**
   * @brief Virtual destructor.
   */
  XUZY_API
  virtual ~Package() = 0;

  /**
   * @brief Return the file name.
   * @param int num - The file number.
   */
  XUZY_API
  virtual const char* getFileName(int num) = 0;

  /**
   * @brief Return the number of files in the package.
   * @return Number of the package files.
   */
  XUZY_API
  virtual int getNumFiles() = 0;

  /**
   * @brief Clone the external package for multi-threaded loading.
   * @return The cloned package.
   * Return nullptr if the multi-threading loading is not possible.
   */
  XUZY_API
  virtual Package* clone() = 0;

  /**
   * @brief Read currently selected file.
   * @param unsigned char * data - Destination data pointer.
   * @param int size - Size of the data buffer.
   * @return Returns 1 if the file is successfully read; otherwise, 0 is
   * returned.
   */
  XUZY_API
  virtual int readFile(unsigned char* data, int size) = 0;

  /**
   * @brief Select a file by name.
   * @param const char * name - The file name.
   * @param int & size - The file size.
   * @return Returns 1 if the file is selected; otherwise, 0 is returned.
   */
  XUZY_API
  virtual int selectFile(const char* name, int& size) = 0;
};

}  // namespace xuzy::Filesystem
