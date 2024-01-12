#include <glog/logging.h>

#include "classloader/shared_library.h"

using namespace dynamicclassloader;

SharedLibrary::SharedLibrary()
{
}

SharedLibrary::SharedLibrary(const std::string &path)
{
	loadImpl(path, 0);
}

SharedLibrary::SharedLibrary(const std::string &path, int flags)
{
	loadImpl(path, flags);
}

SharedLibrary::~SharedLibrary()
{
}

void SharedLibrary::load(const std::string &path)
{
	loadImpl(path, 0);
}

void SharedLibrary::load(const std::string &path, int flags)
{
	loadImpl(path, flags);
}

void SharedLibrary::unload()
{
	unloadImpl();
}

bool SharedLibrary::isLoaded() const
{
	return isLoadedImpl();
}

const std::string &SharedLibrary::getPath() const
{
	return getPathImpl();
}

std::string SharedLibrary::suffix()
{
	return suffixImpl();
}

bool SharedLibrary::setSearchPath(const std::string &path)
{
	return setSearchPathImpl(path);
}

bool SharedLibrary::hasSymbol(const std::string &name)
{
	return hasSymbolImpl(name);
}