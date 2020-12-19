#include <criterion/criterion.h>

#include "./test_types.hpp"

#include <container.hpp>

namespace tt = tests::types;

TestSuite(ContainerExceptions, .description = "Try to do expected unexpected calls (i.e. stuff user might try to do).", .disabled = false);

Test(ContainerException, getUnregisteredType, .description = "Try to retrieve a type that was never instanciated in the first place.") {
    clonixin::Container container;
    using namespace clonixin::exceptions;
    using Error = ContainerError;

    class Empty {};

    cr_assert_throw(
            auto ptr = container.getInstance<Empty>();
    , ContainerException<Error::TypeNotFound>);
}

Test(ContainerException, getUnregisteredTypeByValue, .description = "Try to retrieve a type that was never instanciated in the first place.") {
    clonixin::Container container;
    using namespace clonixin::exceptions;
    using Error = ContainerError;

    class Empty {};

    cr_assert_throw(
            auto ptr = container.getInstance<Empty &&>();
    , ContainerException<Error::TypeNotFound>);
}

Test(ContainerException, getRegisteredSingletonByValue, .description = "Try to retrieve a type that was never instanciated in the first place.") {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::exceptions;
    using Error = ContainerError;

    class Empty {};

    container.addType<Singleton<tt::BadSingleton<>>>();

    cr_assert_throw(
            auto ptr = container.getInstance<tt::BadSingleton<>&&>();
    , ContainerException<Error::BadLifetime>);
}

Test(ContainerException, getTransientInterfaceRvalue, .description = "register a transient class without dependencies and retrieve it.") {
    clonixin::Container container;

    class TestClass: public tt::Interface<0> {
        public:
            TestClass(): tt::Interface<0>() {

            }

            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;
    using namespace clonixin::exceptions;
    using Error = clonixin::exceptions::BuilderError;

    container.addType<Transient<tt::Interface<0>, TestClass>>();

    cr_assert_throw(
            const tt::Interface<0> & ptr = container.getInstance<tt::Interface<0> &&>();
    , BuilderException<Error::AbstractRvalue>);
}

Test(ContainerException, getUnmovableRvalue, .description = "register an unmovable type and try to get it by value") {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::exceptions;
    using Error = clonixin::exceptions::BuilderError;

    container.addType<Transient<tt::UnMovableTransient<>>>();

    cr_assert_throw(
        auto const &test = container.getInstance<tt::UnMovableTransient<> &&>();
    , BuilderException<Error::RvalueUnsupported>)
}
