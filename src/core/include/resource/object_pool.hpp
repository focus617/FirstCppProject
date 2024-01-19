/**
 * @brief Object Pool example derived from
 * https://sourcemaking.com/design_patterns/object_pool/cpp/1
 */

#include <iostream>
#include <list>
#include <string>

/**
 * @brief Reusable Resource
 */
class Resource {
 private:
  int value;

 public:
  Resource() { value = 0; }
  void reset() { value = 0; }
  int getValue() { return value; }
  void setValue(int number) { value = number; }
};

/**
 * @brief Resource Pool Manager
 * Note, that this class is a singleton.
 */
class ObjectPool {
 private:
  std::list<Resource*> resources;

  ObjectPool() {}

 public:
  /**
   * @brief Static method for accessing class instance.
   * Part of Singleton design pattern.
   *
   * @return ObjectPool instance.
   */
  static ObjectPool& Instance() {
    static ObjectPool* instance = new ObjectPool();  // inits once (lazy)
    return *instance;
  }

  /**
   * @brief Returns instance of Resource.
   * New resource will be created if all the resources were used at the time of
   * the request.
   *
   * @return Resource instance.
   */
  Resource* getResource() {
    if (resources.empty()) {
      std::cout << "Creating new resource." << std::endl;
      return new Resource;
    } else {
      std::cout << "Reusing existing resource." << std::endl;
      Resource* resource = resources.front();
      resources.pop_front();
      return resource;
    }
  }

  /**
   * @brief Return resource back to the pool.
   * The resource must be initialized back to the default settings before
   * someone else attempts to use it.
   *
   * @param object Resource instance.
   * @return void
   */
  void returnResource(Resource* object) {
    object->reset();
    resources.push_back(object);
  }
};
