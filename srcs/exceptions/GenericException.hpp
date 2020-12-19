
/**
** \file GenericException.hpp
**
** Header file for clonixin GenericException, a convenience type that's used
** to quickly create exceptions.
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-06-02 22:36
** \date Last update: 2020-06-08 15:27
** \copyright GNU Lesser Public Licence v3
*/

#ifndef exceptions_GenericException_hpp__
#define exceptions_GenericException_hpp__

#include <array>
#include <string>

#include "./ClonixinException.hpp"

namespace clonixin::exceptions {
    /**
    ** \brief Default error component.
    */
    inline constexpr char GENERIC_COMPONENT[] = "Generic Error";

    /**
    ** \brief Templated class used to quicly create exceptions.
    **
    ** This type is the default one for all exceptions. It's template parameter
    ** allow type differences based on the type of error. The type of err is
    ** unspecified, so any component only has to declare an enum to represent
    ** each errors, which will create several type, allowing to do pattern
    ** matching with catch blocks.
    **
    ** \tparam err This unspecified type can be replaced by value of a valid
    ** template value. That is, most integer literals, or enum value. It's use
    ** is two-fold: Using enum types enable to differentiate error thrown in
    ** certain modules from the one thrown in other, and each value in the enum
    ** represent a different kind of error, thrown from this module.
    **
    ** \tparam component This pointer on char is there to specify the component
    ** describing string that will be available to retrieve for logging and
    ** debugging purposes.
    */
    template <auto err, char const * const component = GENERIC_COMPONENT>
    class GenericException : public ClonixinException {
        public:
            GenericException(std::string const &what);
            GenericException(std::string const &what, std::string const &file);
            GenericException(std::string const &what, std::string const &file, size_t line);
            virtual ~GenericException() {}

            std::string const &strWhat() const noexcept override;
        private:
            std::string const _what;
    };

    using namespace std::literals;


    /**
    ** \brief GenericException constructor.
    **
    ** This constructor only take an explanatory string of the error that
    ** occurred.
    **
    ** \param what Reference to an explanatory string describing the error.
    **
    ** \tparam err This unspecified type can be replaced by value of a valid
    ** template value. That is, most integer literals, or enum value. It's use
    ** is two-fold: Using enum types enable to differentiate error thrown in
    ** certain modules from the one thrown in other, and each value in the enum
    ** represent a different kind of error, thrown from this module.
    **
    ** \tparam component This pointer on char is there to specify the component
    ** describing string that will be available to retrieve for logging and
    ** debugging purposes.
    */
    template <auto err, char const * const component>
    inline GenericException<err, component>::GenericException(std::string const &what)
    : ClonixinException(component), _what(what) {}

    /**
    ** \brief GenericException constructor.
    **
    ** This constructor take an explanatory string of the error that
    ** occurred, as well as the file in which the error occurred.
    **
    ** \param what Reference to an explanatory string describing the error.
    **
    ** \param file Name of the file from which the error is thrown
    **
    ** \tparam err This unspecified type can be replaced by value of a valid
    ** template value. That is, most integer literals, or enum value. It's use
    ** is two-fold: Using enum types enable to differentiate error thrown in
    ** certain modules from the one thrown in other, and each value in the enum
    ** represent a different kind of error, thrown from this module.
    **
    ** \tparam component This pointer on char is there to specify the component
    ** describing string that will be available to retrieve for logging and
    ** debugging purposes.
    */
    template <auto err, char const * const component>
    inline GenericException<err, component>::GenericException(std::string const &what, std::string const &file)
    : ClonixinException(GENERIC_COMPONENT, file), _what(what) {}

    /**
    ** \brief GenericException constructor.
    **
    ** This constructor take an explanatory string of the error that
    ** occurred, as well as the file in which the error occurred.
    **
    ** \param what Reference to an explanatory string describing the error.
    **
    ** \param file Name of the file from which the error is thrown
    ** \param line Line at which the error is thrown.
    **
    ** \tparam err This unspecified type can be replaced by value of a valid
    ** template value. That is, most integer literals, or enum value. It's use
    ** is two-fold: Using enum types enable to differentiate error thrown in
    ** certain modules from the one thrown in other, and each value in the enum
    ** represent a different kind of error, thrown from this module.
    **
    ** \tparam component This pointer on char is there to specify the component
    ** describing string that will be available to retrieve for logging and
    ** debugging purposes.
    */
    template <auto err, char const * const component>
    inline GenericException<err, component>::GenericException(std::string const &what, std::string const &file, size_t line)
    : ClonixinException(GENERIC_COMPONENT, file, line), _what(what) {}

    /**
    ** \brief Return the exception's explanatory string as a std::string.
    **
    ** \tparam err This unspecified type can be replaced by value of a valid
    ** template value. That is, most integer literals, or enum value. It's use
    ** is two-fold: Using enum types enable to differentiate error thrown in
    ** certain modules from the one thrown in other, and each value in the enum
    ** represent a different kind of error, thrown from this module.
    **
    ** \tparam component This pointer on char is there to specify the component
    ** describing string that will be available to retrieve for logging and
    ** debugging purposes.
    **
    ** \return Explanatory string of the exception, as a std::string.
    */
    template <auto err, char const * const component>
    inline std::string const & GenericException<err, component>::strWhat() const noexcept {
        return _what;
    }
}

#endif
