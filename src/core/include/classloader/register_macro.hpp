#pragma once

#define CLASS_LOADER_REGISTER_CLASS_INTERNAL_WITH_MESSAGE(Derived, Base,     \
                                                          UniqueID, Message) \
  namespace {                                                                \
  struct ProxyExec##UniqueID {                                               \
    typedef Derived _derived;                                                \
    typedef Base _base;                                                      \
    ProxyExec##UniqueID() {                                                  \
      if (!std::string(Message).empty()) {                                   \
        LOG(INFO) << Message;                                                \
      }                                                                      \
      class_loader::impl::registerPlugin<_derived, _base>(#Derived, #Base);  \
    }                                                                        \
  };                                                                         \
  static ProxyExec##UniqueID g_register_plugin_##UniqueID;                   \
  }  // namespace

#define CLASS_LOADER_REGISTER_CLASS_INTERNAL_HOP1_WITH_MESSAGE(              \
    Derived, Base, UniqueID, Message)                                        \
  CLASS_LOADER_REGISTER_CLASS_INTERNAL_WITH_MESSAGE(Derived, Base, UniqueID, \
                                                    Message)

/**
 * @macro This macro is same as CLASS_LOADER_REGISTER_CLASS, but will spit out a
 * message when the plugin is registered at library load time
 */
#define CLASS_LOADER_REGISTER_CLASS_WITH_MESSAGE(Derived, Base, Message) \
  CLASS_LOADER_REGISTER_CLASS_INTERNAL_HOP1_WITH_MESSAGE(Derived, Base,  \
                                                         __COUNTER__, Message)

/**
 * @macro This is the macro which must be declared within the source (.cpp) file
 * for each class that is to be exported as plugin. The macro utilizes a trick
 * where a new struct is generated along with a declaration of static global
 * variable of same type after it. The struct's constructor invokes a
 * registration function with the plugin system. When the plugin system loads a
 * library with registered classes in it, the initialization of static variables
 * forces the invocation of the struct constructors, and all exported classes
 * are automatically registerd.
 */
#define CLASS_LOADER_REGISTER_CLASS(Derived, Base) \
  CLASS_LOADER_REGISTER_CLASS_WITH_MESSAGE(Derived, Base, "")
