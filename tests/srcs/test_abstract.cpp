#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerAbstract, .description = "Testing container abstract type support.", .disabled = false);

Test(ContainerAbstract, getTransientInterfaceNoDependencies, .description = "register a transient class without dependencies and retrieve it.") {
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

    container.addType<Transient<tt::Interface<0>, TestClass>>();

    std::shared_ptr<tt::Interface<0>> ptr = container.getInstance<tt::Interface<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getTransientInterfaceWithDependencies, .description = "register a transient class without dependencies and retrieve it.") {
    clonixin::Container container;

    class TestClass: public tt::Interface<0> {
        public:
            TestClass(std::shared_ptr<tt::Transient<>>): tt::Interface<0>() {

            }

            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;

    container.addType<Transient<tt::Interface<0>, TestClass>, tt::Transient<>>();
    container.addType<Transient<tt::Transient<>>>();

    std::shared_ptr<tt::Interface<0>> ptr = container.getInstance<tt::Interface<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getTransientAbstractNoDependencies, .description = "register a transient class without dependencies and retrieve .") {
    clonixin::Container container;

    class TestClass: public tt::Abstract<0> {
        public:
            TestClass(): tt::Abstract<0>() {

            }

            virtual ~TestClass() {}
    };

    using namespace clonixin::type_desc;

    container.addType<Transient<tt::Abstract<0>, TestClass>>();

    std::shared_ptr<tt::Abstract<0>> ptr = container.getInstance<tt::Abstract<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getTransientAbstractWithDependencies, .description = "register a transient class without dependencies and retrieve it.") {
    clonixin::Container container;

    class TestClass: public tt::Abstract<0> {
        public:
            TestClass(std::shared_ptr<tt::Transient<>>): tt::Abstract<0>() {

            }

            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;

    container.addType<Transient<tt::Abstract<0>, TestClass>, tt::Transient<>>();
    container.addType<Transient<tt::Transient<>>>();

    std::shared_ptr<tt::Abstract<0>> ptr = container.getInstance<tt::Abstract<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getSingletonInterfaceNoDependencies, .description = "register a singleton class without dependencies and retrieve it.") {
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

    container.addType<Singleton<tt::Interface<0>, TestClass>>();

    std::shared_ptr<tt::Interface<0>> ptr = container.getInstance<tt::Interface<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getSingletonInterfaceWithDependencies, .description = "register a transient class without dependencies and retrieve it.") {
    clonixin::Container container;

    class TestClass: public tt::Interface<0> {
        public:
            TestClass(std::shared_ptr<tt::Singleton<>>): tt::Interface<0>() {

            }

            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;

    container.addType<Singleton<tt::Interface<0>, TestClass>, tt::Singleton<>>();
    container.addType<Singleton<tt::Singleton<>>>();

    std::shared_ptr<tt::Interface<0>> ptr = container.getInstance<tt::Interface<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getSingletonAbstractNoDependencies, .description = "register a singleton class without dependencies and retrieve .") {
    clonixin::Container container;

    class TestClass: public tt::Abstract<0> {
        public:
            TestClass(): tt::Abstract<0>() {

            }

            virtual ~TestClass() {}
    };

    using namespace clonixin::type_desc;

    container.addType<Singleton<tt::Abstract<0>, TestClass>>();

    std::shared_ptr<tt::Abstract<0>> ptr = container.getInstance<tt::Abstract<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

Test(ContainerAbstract, getSingletonAbstractWithDependencies, .description = "register a transient class without dependencies and retrieve it.") {
    clonixin::Container container;

    class TestClass: public tt::Abstract<0> {
        public:
            TestClass(std::shared_ptr<tt::Singleton<>>): tt::Abstract<0>() {

            }

            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;

    container.addType<Singleton<tt::Abstract<0>, TestClass>, tt::Singleton<>>();
    container.addType<Singleton<tt::Singleton<>>>();

    std::shared_ptr<tt::Abstract<0>> ptr = container.getInstance<tt::Abstract<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    ptr->getDiscr();
}

