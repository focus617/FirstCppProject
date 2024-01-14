#pragma once

#include <stdexcept>

#include "visibility_control.hpp"
namespace xuzy
{

    ///
    /// \brief This is the base class for all exceptions
    ///
    class XUZY_API Exception : public std::exception
    {
    public:
        ///
        /// \brief Creates an exception.
        /// \param
        ///
        Exception(const std::string &msg, int code = 0);

        ///
        /// \brief Creates an exception.
        /// \param
        ///
        Exception(const std::string &msg, const std::string &arg, int code = 0);

        ///
        /// \brief Creates an exception and stores a clone of the nested
        /// exception.
        /// \param
        ///
        Exception(const std::string &msg, const Exception &nested,
                  int code = 0);

        ///
        /// \brief Copy constructor.
        /// \param
        ///
        Exception(const Exception &exc);

        ///
        /// \brief Destroys the exception and deletes the nested exception.
        /// \param
        ///
        ~Exception() noexcept;

        ///
        /// \brief Assignment operator.
        /// \param
        ///
        Exception &operator=(const Exception &exc);

        ///
        /// \brief
        /// \return a static string describing the exception.
        ///
        virtual const char *name() const noexcept;

        ///
        /// \brief
        /// \return the name of the exception class.
        ///
        virtual const char *className() const noexcept;

        ///
        /// \brief
        /// \return a static string describing the exception.
        ///
        /// Same as name(), but for compatibility with std::exception.
        ///
        virtual const char *what() const noexcept;

        ///
        /// \brief
        /// \return a pointer to the nested exception, or null if no nested
        /// exception exists.
        ///
        const Exception *nested() const;

        ///
        /// \brief
        /// \return the message text.
        ///
        const std::string &message() const;

        ///
        /// \brief
        /// \return the exception code if defined.
        ///
        int code() const;

        ///
        /// \brief
        /// \return a string consisting of the message name and the message
        ///  text.
        ///
        std::string displayText() const;

        ///
        /// \brief Creates an exact copy of the exception.
        ///
        /// The copy can later be thrown again by invoking rethrow() on it.
        ///
        virtual Exception *clone() const;

        ///
        /// \brief (Re)Throws the exception.
        ///
        /// This is useful for temporarily storing a copy of an exception
        ///  (see clone()), then throwing it again.
        ///
        virtual void rethrow() const;

    protected:
        ///
        /// \brief Standard constructor.
        /// \param
        ///
        Exception(int code = 0);

        ///
        /// \brief Sets the message for the exception.
        /// \param
        ///
        void message(const std::string &msg);

        ///
        /// \brief Sets the extended message for the exception.
        /// \param arg The message extended to exception
        ///
        void extendedMessage(const std::string &arg);

    private:
        std::string _msg;
        Exception *_pNested;
        int _code;
    };

    //
    // inlines
    //
    inline const Exception *Exception::nested() const
    {
        return _pNested;
    }

    inline const std::string &Exception::message() const
    {
        return _msg;
    }

    inline void Exception::message(const std::string &msg)
    {
        _msg = msg;
    }

    inline int Exception::code() const
    {
        return _code;
    }

//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define XUZY_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE)                       \
    class API XUZY_API CLS : public BASE                                      \
    {                                                                         \
    public:                                                                   \
        CLS(int code = CODE);                                                 \
        CLS(const std::string &msg, int code = CODE);                         \
        CLS(const std::string &msg, const std::string &arg, int code = CODE); \
        CLS(const std::string &msg, const xuzy::Exception &exc,               \
            int code = CODE);                                                 \
        CLS(const CLS &exc);                                                  \
        ~CLS() noexcept;                                                      \
        CLS &operator=(const CLS &exc);                                       \
        const char *name() const noexcept;                                    \
        const char *className() const noexcept;                               \
        xuzy::Exception *clone() const;                                       \
        void rethrow() const;                                                 \
    };

#define XUZY_DECLARE_EXCEPTION(API, CLS, BASE) \
    XUZY_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)
    
#define XUZY_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                      \
    CLS::CLS(int code) : BASE(code)                                    \
    {                                                                  \
    }                                                                  \
    CLS::CLS(const std::string &msg, int code) : BASE(msg, code)       \
    {                                                                  \
    }                                                                  \
    CLS::CLS(const std::string &msg, const std::string &arg, int code) \
        : BASE(msg, arg, code)                                         \
    {                                                                  \
    }                                                                  \
    CLS::CLS(                                                          \
        const std::string &msg,                                        \
        const xuzy::Exception &exc,                                    \
        int code)                                                      \
        : BASE(msg, exc, code)                                         \
    {                                                                  \
    }                                                                  \
    CLS::CLS(const CLS &exc) : BASE(exc)                               \
    {                                                                  \
    }                                                                  \
    CLS::~CLS() noexcept                                               \
    {                                                                  \
    }                                                                  \
    CLS &CLS::operator=(const CLS &exc)                                \
    {                                                                  \
        BASE::operator=(exc);                                          \
        return *this;                                                  \
    }                                                                  \
    const char *CLS::name() const noexcept                             \
    {                                                                  \
        return NAME;                                                   \
    }                                                                  \
    const char *CLS::className() const noexcept                        \
    {                                                                  \
        return typeid(*this).name();                                   \
    }                                                                  \
    xuzy::Exception *CLS::clone() const                                \
    {                                                                  \
        return new CLS(*this);                                         \
    }                                                                  \
    void CLS::rethrow() const                                          \
    {                                                                  \
        throw *this;                                                   \
    }

    //
    // Standard exception classes
    //
    XUZY_DECLARE_EXCEPTION(XUZY_API, ApplicationException, Exception)

    XUZY_DECLARE_EXCEPTION(XUZY_API, LogicException, Exception)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, AssertionViolationException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NullPointerException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NullValueException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, BugcheckException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, InvalidArgumentException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NotImplementedException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, RangeException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, IllegalStateException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, InvalidAccessException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, SignalException, LogicException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, UnhandledException, LogicException)

    XUZY_DECLARE_EXCEPTION(XUZY_API, RuntimeException, Exception)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NotFoundException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, ExistsException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, TimeoutException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, SystemException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, RegularExpressionException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, LibraryLoadException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, LibraryUnLoadException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, LibraryAlreadyLoadedException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NoThreadAvailableException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, PropertyNotSupportedException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, PoolOverflowException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, NoPermissionException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, OutOfMemoryException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, UnknownURISchemeException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, TooManyURIRedirectsException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, BadCastException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(
        XUZY_API, CreateClassException, RuntimeException)

    XUZY_DECLARE_EXCEPTION(XUZY_API, DataException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, DataFormatException, DataException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, CircularReferenceException, DataException)

    XUZY_DECLARE_EXCEPTION(XUZY_API, SyntaxException, DataException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, PathSyntaxException, SyntaxException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, URISyntaxException, SyntaxException)

    XUZY_DECLARE_EXCEPTION(XUZY_API, IOException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, ProtocolException, IOException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileException, IOException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileExistsException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileNotFoundException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, PathNotFoundException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileReadOnlyException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileAccessDeniedException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, CreateFileException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, OpenFileException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, WriteFileException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, ReadFileException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, FileNotReadyException, FileException)
    XUZY_DECLARE_EXCEPTION(XUZY_API, DirectoryNotEmptyException, FileException)

} // namespace xuzy
