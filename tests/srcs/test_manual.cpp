#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerManual, .description = "Testing basic container features: Retrieving shared_ptr on types.", .disabled = false);

Test(ContainerManual, getTransientNoDependenciesOnce, .description = "register a simple instance of a transient object and build it.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Transient<>;

    container.addTransient(std::make_unique<GenericBuilder<TestClass>>());

    std::shared_ptr<tt::Transient<>> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerManual, getTransientNoDependenciesTwoTimes, .description = "register a simple instance of a transient object and build it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Transient<>;

    container.addTransient(std::make_unique<GenericBuilder<TestClass>>());

    std::shared_ptr<tt::Transient<>> ptr = container.getInstance<TestClass>();
    std::shared_ptr<tt::Transient<>> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert_not(ptr == ptr2, "Returned pointers should refer to different objects.");
    cr_assert_eq(TestClass::getCount(), 2, "Number of TestClass instances should be 2.");
}

Test(ContainerManual, getTransientWithTransientDependenciesOnce, .description = "Register several classes with dependencies and retrieve one of them once.", .disabled = false) {
    clonixin::Container container;
    class Empty {};

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addTransient(std::make_unique<GenericBuilder<Empty>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency2, Empty>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addTransient(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerManual, getTransientWithTransientDependenciesTwoTimes, .description = "Register several classes with dependencies and retrieve one of them twice.", .disabled = false) {
    clonixin::Container container;

    class Empty {};

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addTransient(std::make_unique<GenericBuilder<Empty>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency2, Empty>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addTransient(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();
    std::shared_ptr<TestClass> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_not(ptr == ptr2);
    cr_assert_eq(Dependency::getCount(), 2, "Dependency should be built exactly two times. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 2, "Dependency2 should be built exactly two times. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 2, "Dependency3 should be built exactly two times. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 2, "TestClass should be built exactly two times. Built %d time(s)", TestClass::getCount());
}

Test(ContainerManual, getTransientWithMixedDependencies, .description = "Register several classes with dependencies and retrieve one them once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Dependency>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency2>, std::shared_ptr<Dependency3>>;

    container
        .addTransient(std::make_unique<GenericBuilder<Dependency>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency2, Dependency>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addTransient(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency2, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();
    std::shared_ptr<TestClass> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_not(ptr == ptr2, "Returend pointers should not be equals.");
    cr_assert_eq(Dependency::getCount(), 3, "Dependency should be built exactly three times. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 2, "TestClass should be built exactly two times. Built %d time(s)", TestClass::getCount());
}


Test(ContainerManual, getSingletonNoDependenciesOnce, .description = "register a simple instance of a singleton object and retrieve it once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Singleton<>;

    container.addSingleton(std::make_unique<GenericBuilder<TestClass>>());

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerManual, getSingletonNoDependenciesTwoTime, .description = "Register a simple instance of a singleton object and retrieve it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Singleton<>;

    container.addSingleton(std::make_unique<GenericBuilder<TestClass>>());

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();
    std::shared_ptr<tt::Singleton<>> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert(ptr == ptr2, "Returned pointers should be equals.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerManual, getSingletonWithSingletonDependenciesOnce, .description = "Register several classes with dependencies and retrieve one of them once.", .disabled = false) {
    clonixin::Container container;
    class Empty {};

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Singleton<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addSingleton(std::make_unique<GenericBuilder<Empty>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency2, Empty>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addSingleton(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerManual, getSingletonWithSingletonDependenciesTwoTimes, .description = "Register several classes with dependencies and retrieve one of them twice.", .disabled = false) {
    clonixin::Container container;

    class Empty {};

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Singleton<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addSingleton(std::make_unique<GenericBuilder<Empty>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency2, Empty>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addSingleton(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();
    std::shared_ptr<TestClass> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert(ptr == ptr2, "Returned pointer should be equals.");
    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerManual, getSingletonWithMixedDependencies, .description = "Register several classes with dependencies and retrieve one them once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Dependency>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency2>, std::shared_ptr<Dependency3>>;

    container
        .addTransient(std::make_unique<GenericBuilder<Dependency>>())
        .addSingleton(std::make_unique<GenericBuilder<Dependency2, Dependency>>())
        .addTransient(std::make_unique<GenericBuilder<Dependency3, Dependency2>>())
        .addSingleton(std::make_unique<GenericBuilder<TestClass, Dependency, Dependency2, Dependency3>>())
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();
    std::shared_ptr<TestClass> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(ptr == ptr2, "Returned pointers should be equals.");
    cr_assert_eq(Dependency::getCount(), 2, "Dependency should be built exactly two times. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerManual, getInstanceOnce, .description = "register a simple instance of a singleton object and retrieve it once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Singleton<>;

    container.addInstance(std::make_unique<TestClass>());

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerManual, getInstanceTwoTime, .description = "Register a simple instance of a singleton object and retrieve it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;
    using TestClass = tt::Singleton<>;

    container.addInstance(std::make_unique<TestClass>());

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();
    std::shared_ptr<tt::Singleton<>> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert(ptr == ptr2, "Returned pointers should be equals.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerManual, getInterfaceTransientNoDependenciesOnce, .description = "register a simple instance of a transient object and build it.", .disabled = false) {
    clonixin::Container container;

    class TestClass : public tt::Interface<0>{
        public:
            TestClass() {}
            virtual ~TestClass() {}

            virtual int getDiscr() const override {
                return 0;
            }
    };

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    container.addTransient(std::make_unique<AbstractBuilder<tt::Interface<0>, TestClass>>());

    std::shared_ptr<tt::Interface<0>> ptr = container.getInstance<tt::Interface<0>>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
}
