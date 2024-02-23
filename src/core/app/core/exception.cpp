#include "core/exception.hpp"

#include <glog/logging.h>

#include <typeinfo>

using namespace xuzy;

Exception::Exception(int code) : _pNested(0), _code(code) {}

Exception::Exception(const std::string& msg, int code)
    : _msg(msg), _pNested(0), _code(code) {}

Exception::Exception(const std::string& msg, const std::string& arg, int code)
    : _msg(msg), _pNested(0), _code(code) {
  if (!arg.empty()) {
    _msg.append(": ");
    _msg.append(arg);
  }
}

Exception::Exception(const std::string& msg, const Exception& nested, int code)
    : _msg(msg), _pNested(nested.clone()), _code(code) {}

Exception::Exception(const Exception& exc)
    : std::exception(exc), _msg(exc._msg), _code(exc._code) {
  _pNested = exc._pNested ? exc._pNested->clone() : 0;
}

Exception::~Exception() noexcept { delete _pNested; }

Exception& Exception::operator=(const Exception& exc) {
  if (&exc != this) {
    Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
    delete _pNested;
    _msg = exc._msg;
    _pNested = newPNested;
    _code = exc._code;
  }
  return *this;
}

const char* Exception::name() const noexcept { return "Exception"; }

const char* Exception::className() const noexcept {
  return typeid(*this).name();
}

const char* Exception::what() const noexcept { return name(); }

std::string Exception::displayText() const {
  std::string txt = name();
  if (!_msg.empty()) {
    txt.append(": ");
    txt.append(_msg);
  }
  return txt;
}

void Exception::extendedMessage(const std::string& arg) {
  if (!arg.empty()) {
    if (!_msg.empty()) _msg.append(": ");
    _msg.append(arg);
  }
}

Exception* Exception::clone() const { return new Exception(*this); }

void Exception::rethrow() const { throw *this; }

XUZY_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
XUZY_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException,
                         "Assertion violation")
XUZY_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
XUZY_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
XUZY_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
XUZY_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException,
                         "Invalid argument")
XUZY_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException,
                         "Not implemented")
XUZY_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
XUZY_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
XUZY_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException,
                         "Invalid access")
XUZY_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
XUZY_IMPLEMENT_EXCEPTION(UnhandledException, LogicException,
                         "Unhandled exception")

XUZY_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
XUZY_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
XUZY_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
XUZY_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
XUZY_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
XUZY_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException,
                         "Error in regular expression")
XUZY_IMPLEMENT_EXCEPTION(LibraryNotFoundException, RuntimeException,
                         "Cannot find library in path")
XUZY_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException,
                         "Cannot load library")
XUZY_IMPLEMENT_EXCEPTION(LibraryUnLoadException, RuntimeException,
                         "Cannot unload library")
XUZY_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException,
                         "Library already loaded")
XUZY_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException,
                         "No thread available")
XUZY_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException,
                         "Property not supported")
XUZY_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException,
                         "Pool overflow")
XUZY_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException,
                         "No permission")
XUZY_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException,
                         "Out of memory")
XUZY_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

XUZY_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
XUZY_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
XUZY_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException,
                         "Circular reference")
XUZY_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException,
                         "Bad path syntax")
XUZY_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
XUZY_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
XUZY_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
XUZY_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
XUZY_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
XUZY_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
XUZY_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException,
                         "File is read-only")
XUZY_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException,
                         "Access to file denied")
XUZY_IMPLEMENT_EXCEPTION(CreateFileException, FileException,
                         "Cannot create file")
XUZY_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
XUZY_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
XUZY_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
XUZY_IMPLEMENT_EXCEPTION(FileNotReadyException, FileException, "File not ready")
XUZY_IMPLEMENT_EXCEPTION(DirectoryNotEmptyException, FileException,
                         "Directory not empty")
XUZY_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException,
                         "Unknown URI scheme")
XUZY_IMPLEMENT_EXCEPTION(TooManyURIRedirectsException, RuntimeException,
                         "Too many URI redirects")
XUZY_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")

XUZY_IMPLEMENT_EXCEPTION(ApplicationException, Exception,
                         "Application exception")
XUZY_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException,
                         "Bad cast exception")