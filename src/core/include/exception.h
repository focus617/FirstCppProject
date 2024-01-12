#pragma once

#include <stdexcept>

namespace xuzy
{

    ///
    /// \brief This is the base class for all exceptions
    ///
    class Exception : public std::exception
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
#define XUZY_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE)                          \
    class CLS : public BASE                                                   \
    {                                                                         \
    public:                                                                   \
        CLS(int code = CODE);                                                 \
        CLS(const std::string &msg, int code = CODE);                         \
        CLS(const std::string &msg, const std::string &arg, int code = CODE); \
        CLS(const std::string &msg, const xuzy::Exception &exc, \
            int code = CODE);                                                 \
        CLS(const CLS &exc);                                                  \
        ~CLS() noexcept;                                                      \
        CLS &operator=(const CLS &exc);                                       \
        const char *name() const noexcept;                                    \
        const char *className() const noexcept;                               \
        xuzy::Exception *clone() const;                         \
        void rethrow() const;                                                 \
    };

#define XUZY_DECLARE_EXCEPTION(CLS, BASE) \
    XUZY_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

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
        const xuzy::Exception &exc,                      \
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
    xuzy::Exception *CLS::clone() const                  \
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
    XUZY_DECLARE_EXCEPTION(RuntimeException, Exception)
    XUZY_DECLARE_EXCEPTION(NotFoundException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
    XUZY_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)

} // namespace xuzy
