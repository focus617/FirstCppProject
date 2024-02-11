#pragma once

#include <mutex>

#include "platform.hpp"
#include "stream_formatter.hpp"

namespace xuzy {

/**
 * @brief A copyable object representing a user specified property which can be
 * output as a key/value string pair.
 *
 * Don't inherit from Property as its destructor is not virtual.
 */
class Property {
 public:
  // C'tor.  Property does NOT have a default constructor.
  // Always use this constructor (with parameters) to create a
  // Property object.
  Property(const std::string& a_key, const std::string& a_value)
      : key_(a_key), value_(a_value) {}

  // Gets the user supplied key.
  const char* key() const { return key_.c_str(); }

  // Gets the user supplied value.
  const char* value() const { return value_.c_str(); }

  // Sets a new value, overriding the one supplied in the constructor.
  void set_value(const std::string& new_value) { value_ = new_value; }

 private:
  // The key supplied by the user.
  std::string key_;
  // The value supplied by the user.
  std::string value_;
};

/**
 * @brief A predicate that checks the key of a Property against a known key.
 *
 * PropertyKeyIs is copyable.
 */
class PropertyKeyIs {
 public:
  // Constructor.
  //
  // PropertyKeyIs has NO default constructor.
  explicit PropertyKeyIs(const std::string& key) : key_(key) {}

  // Returns true if and only if the key of property matches on key_.
  bool operator()(const Property& property) const {
    return property.key() == key_;
  }

 private:
  std::string key_;
};

class Properties {
 public:
  /**
   * @brief Gets the vector of TestProperties.
   */
  const std::vector<Property>& get_properties() const { return properties_; }

  /**
   * @brief Returns the number of the test properties.
   */
  int property_count() const { return static_cast<int>(properties_.size()); }

  /**
   * @brief Clears the object.
   */
  void Clear() { properties_.clear(); }

  /**
   * @brief Returns the i-th test property.
   * @param i can range from 0 to property_count() - 1.
   * If i is not in that range, aborts the program.
   */
  const Property& get_property(int i) const {
    if (i < 0 || i >= property_count()) posix::Abort();
    return properties_.at(static_cast<size_t>(i));
  }

  /**
   * @brief Adds a property to the Properties vector.
   * If a property with the same key as the supplied property is already
   * represented, the value of this property replaces the old value for that
   * key.
   * @param property to be added
   */
  void record_property(const Property& property) {
    // Check if new property has a valid key
    if (!ValidateProperty(property)) {
      return;
    }

    std::lock_guard lock(properties_mutex_);
    const std::vector<Property>::iterator property_with_matching_key =
        std::find_if(properties_.begin(), properties_.end(),
                     PropertyKeyIs(property.key()));
    if (property_with_matching_key == properties_.end()) {
      properties_.push_back(property);
      return;
    }
    property_with_matching_key->set_value(property.value());
  }

  // Produces a string representing the properties in a result as
  // a JSON dictionary.
  std::string PropertiesAsJson(const std::string& indent) {
    Stream_Formatter attributes;
  //   for (int i = 0; i < result.test_property_count(); ++i) {
  //     const TestProperty& property = result.GetTestProperty(i);
  //     attributes << ",\n"
  //                << indent << "\"" << property.key() << "\": "
  //                << "\"" << EscapeJson(property.value()) << "\"";
  //   }
    return attributes.GetString();
  }

 protected:
  /**
   * @brief Adds a failure if the key is a reserved attribute.
   * @return Returns true if the property is valid.
   * @param property to be validate
   */
  virtual bool ValidateProperty(const Property& property) { return true; }

 private:
  /** @brief The vector of Properties */
  std::vector<Property> properties_;
  /**
   * @brief Protects mutable state of the property vector and of owned
   * properties, whose values may be updated.
   */
  std::mutex properties_mutex_;
};

}  // namespace xuzy