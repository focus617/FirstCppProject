#include "classloader/meta_object.hpp"

#include <glog/logging.h>

#include <string>

#include "classloader/class_loader.hpp"
#include "exception.hpp"

namespace class_loader {
namespace impl {

AbstractMetaObjectBase::AbstractMetaObjectBase(
    const std::string& class_name, const std::string& base_class_name)
    : associated_library_path_("Unknown"),
      base_class_name_(base_class_name),
      class_name_(class_name),
      typeid_base_class_name_("UNSET") {
  LOG(INFO) << "class_loader.impl.AbstractMetaObjectBase: "
            << "Creating MetaObject " << this
            << " (base = " << baseClassName().c_str()
            << ", derived = " << className().c_str()
            << ", library path = " << getAssociatedLibraryPath().c_str() << ")";
}

AbstractMetaObjectBase::~AbstractMetaObjectBase() {
  LOG(INFO) << "class_loader.impl.AbstractMetaObjectBase: "
            << "Destroying MetaObject " << this
            << " (base = " << baseClassName().c_str()
            << ", derived = " << className().c_str()
            << ", library path = " << getAssociatedLibraryPath().c_str() << ")";
}

std::string AbstractMetaObjectBase::className() const { return class_name_; }

std::string AbstractMetaObjectBase::baseClassName() const {
  return base_class_name_;
}

std::string AbstractMetaObjectBase::typeidBaseClassName() const {
  return typeid_base_class_name_;
}

std::string AbstractMetaObjectBase::getAssociatedLibraryPath() {
  return associated_library_path_;
}

void AbstractMetaObjectBase::setAssociatedLibraryPath(
    std::string library_path) {
  associated_library_path_ = library_path;
}

void AbstractMetaObjectBase::addOwningClassLoader(ClassLoader* loader) {
  ClassLoaderVector& v = associated_class_loaders_;

  if (std::find(v.begin(), v.end(), loader) == v.end()) {
    v.push_back(loader);
  }
}

void AbstractMetaObjectBase::removeOwningClassLoader(
    const ClassLoader* loader) {
  ClassLoaderVector& v = associated_class_loaders_;

  ClassLoaderVector::iterator itr = std::find(v.begin(), v.end(), loader);
  if (itr != v.end()) {
    v.erase(itr);
  }
}

bool AbstractMetaObjectBase::isOwnedBy(const ClassLoader* loader) {
  ClassLoaderVector& v = associated_class_loaders_;

  ClassLoaderVector::iterator itr = std::find(v.begin(), v.end(), loader);
  return itr != v.end();
}

bool AbstractMetaObjectBase::isOwnedByAnybody() {
  return associated_class_loaders_.size() > 0;
}

ClassLoaderVector AbstractMetaObjectBase::getAssociatedClassLoaders() {
  return associated_class_loaders_;
}

}  // namespace impl
}  // namespace class_loader
