/**
** \file ClonixinException.hpp
**
** Header file for ClonixinException, the base class of all exceptions raised
** by clonixin framework.
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-06-02 21:06
** \date Last update: 2020-06-08 21:30
** \copyright GNU Lesser Public Licence v3
*/

#ifndef exception_ClonixinException_hpp__
#define exception_ClonixinException_hpp__

#include <optional>
#include <string>

/**
** \brief Namespace of all exceptions thrown by the framework
*/
namespace clonixin::exceptions {
    /**
    ** \brief Base class, to catch all exceptions thrown by the framework.
    **
    ** This class inherit from std::exception and is the base for all
    ** exceptions thrown by the framework.
    **
    ** It's meant to be used when it's more convenient to catch any exceptions.
    **
    ** To enable easier debugging, it can also contains optional info, such as
    ** the file and line at which the exception occurred. These informations
    ** does not appear in the "what" message, but can be retrieved if needed.
    */

    class ClonixinException : public std::exception {
        public:
            ClonixinException(std::string const &component,
                std::optional<std::string> const &file = std::nullopt,
                std::optional<size_t> const &line = std::nullopt
            );
            virtual ~ClonixinException();

            virtual char const * what() const noexcept;
            virtual std::string const & strWhat() const noexcept = 0;

            virtual std::optional<std::string> const & getFile() const noexcept final;
            virtual std::optional<size_t> const & getLine() const noexcept final;
        protected:
            std::optional<std::string> _file;
            std::optional<size_t> _line;
            std::string _component;
    };

    /**
    ** \brief ClonixinException constructor.
    **
    ** \param component A string describing what part of clonixin has thrown.
    ** \param file Optional. Name of the file in which the exception was
    ** thrown. If unspecified, default to std::nullopt.
    **
    ** \param line Optional. Line at which the exception was happened. If
    ** unspecified, default to std::nullopt.
    */
    inline ClonixinException::ClonixinException(std::string const & component,
            std::optional<std::string> const &file,
            std::optional<size_t> const & line) : _component(component), _file(file), _line(line)
    {}

    /**
    ** \brief ClonixinException desctructor.
    */
    inline ClonixinException::~ClonixinException() {}

    /**
    ** \brief Exception explanatory string.
    **
    ** \return Pointer to a null-terminated string with explanatory information.
    ** The pointer is valid until the end of the exception's lifetime.
    */
    inline char const * ClonixinException::what() const noexcept {
        return strWhat().c_str();
    }

    /**
    ** \fn std::string const & ClonixinException::strWhat() const noexcept
    ** \brief Return the exception explanatory string.
    **
    ** \return The explanatory string of the current exception. This string is
    ** mostly aimed at the user, if logging is needed.
    */

    /**
    ** \brief Optionally return a string containing the name of the file from
    ** which the exception was thrown.
    **
    ** \return A std::optional that may contains the name of the file from
    ** which the exception came.
    */
    inline std::optional<std::string> const & ClonixinException::getFile() const noexcept {
        return _file;
    }

    /**
    ** \brief Optionally return a string containing the line at which the
    ** exception was thrown.
    **
    ** \return A std::optional that may contains the line at which the
    ** exception happened.
    */
    inline std::optional<size_t> const & ClonixinException::getLine() const noexcept {
        return _line;
    }

    /**
    ** \property std::string ClonixinException::_component
    ** \brief The module in from which the exception occurred.
    */

    /**
    ** \property std::optional<std::string> ClonixinException::_file
    ** \brief The file from which the exception was thrown.
    */

    /**
    ** \property std::optional<size_t> ClonixinException::_line
    ** \brief The line at which the exception happened.
    */
}

#endif
