#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerBasics, .description = "Testing basic container features: Retrieving shared_ptr on types.", .disabled = false);

Test(ContainerBasics, getTransientNoDependenciesOnce, .description = "register a simple instance of a transient object and build it.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Transient<>;

    container.addType<Transient<TestClass>>();

    std::shared_ptr<tt::Transient<>> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerBasics, getTransientNoDependenciesTwoTimes, .description = "register a simple instance of a transient object and build it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Transient<>;

    container.addType<Transient<TestClass>>();

    std::shared_ptr<tt::Transient<>> ptr = container.getInstance<TestClass>();
    std::shared_ptr<tt::Transient<>> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert_not(ptr == ptr2, "Returned pointers should refer to different objects.");
    cr_assert_eq(TestClass::getCount(), 2, "Number of TestClass instances should be 2.");
}

Test(ContainerBasics, getTransientWithTransientDependenciesOnce, .description = "Register several classes with dependencies and retrieve one of them once.", .disabled = false) {
    clonixin::Container container;
    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addType<Transient<Empty>>()
        .addType<Transient<Dependency>>()
        .addType<Transient<Dependency2>, Empty>()
        .addType<Transient<Dependency3>, Dependency2>()
        .addType<Transient<TestClass>, Dependency, Dependency3>()
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerBasics, getTransientWithTransientDependenciesTwoTimes, .description = "Register several classes with dependencies and retrieve one of them twice.", .disabled = false) {
    clonixin::Container container;

    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addType<Transient<Empty>>()
        .addType<Transient<Dependency>>()
        .addType<Transient<Dependency2>, Empty>()
        .addType<Transient<Dependency3>, Dependency2>()
        .addType<Transient<TestClass>, Dependency, Dependency3>()
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

Test(ContainerBasics, getTransientWithMixedDependencies, .description = "Register several classes with dependencies and retrieve one them once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Dependency>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<std::shared_ptr<Dependency>, std::shared_ptr<Dependency2>, std::shared_ptr<Dependency3>>;

    container
        .addType<Transient<Dependency>>()
        .addType<Singleton<Dependency2>, Dependency>()
        .addType<Singleton<Dependency3>, Dependency2>()
        .addType<Transient<TestClass>, Dependency, Dependency2, Dependency3>()
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

Test(ContainerBasics, getSingletonNoDependenciesOnce, .description = "register a simple instance of a singleton object and retrieve it once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Singleton<>;

    container.addType<Singleton<TestClass>>();

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerBasics, getSingletonNoDependenciesTwoTime, .description = "Register a simple instance of a singleton object and retrieve it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Singleton<>;

    container.addType<Singleton<TestClass>>();

    std::shared_ptr<tt::Singleton<>> ptr = container.getInstance<TestClass>();
    std::shared_ptr<tt::Singleton<>> ptr2 = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert(bool(ptr2), "Returned pointer should not be NULL.");
    cr_assert(ptr == ptr2, "Returned pointers should be equals.");
    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerBasics, getSingletonWithSingletonDependenciesOnce, .description = "Register several classes with dependencies and retrieve one of them once.", .disabled = false) {
    clonixin::Container container;
    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Singleton<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addType<Singleton<Empty>>()
        .addType<Singleton<Dependency>>()
        .addType<Singleton<Dependency2>, Empty>()
        .addType<Singleton<Dependency3>, Dependency2>()
        .addType<Singleton<TestClass>, Dependency, Dependency3>()
    ;

    std::shared_ptr<TestClass> ptr = container.getInstance<TestClass>();

    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerBasics, getSingletonWithSingletonDependenciesTwoTimes, .description = "Register several classes with dependencies and retrieve one of them twice.", .disabled = false) {
    clonixin::Container container;

    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Singleton<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Empty>>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency3>>;

    container
        .addType<Singleton<Empty>>()
        .addType<Singleton<Dependency>>()
        .addType<Singleton<Dependency2>, Empty>()
        .addType<Singleton<Dependency3>, Dependency2>()
        .addType<Singleton<TestClass>, Dependency, Dependency3>()
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

Test(ContainerBasics, getSingletonWithMixedDependencies, .description = "Register several classes with dependencies and retrieve one them once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Singleton<std::shared_ptr<Dependency>>;
    using Dependency3 = tt::Transient<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Singleton<std::shared_ptr<Dependency>, std::shared_ptr<Dependency2>, std::shared_ptr<Dependency3>>;

    container
        .addType<Transient<Dependency>>()
        .addType<Singleton<Dependency2>, Dependency>()
        .addType<Transient<Dependency3>, Dependency2>()
        .addType<Singleton<TestClass>, Dependency, Dependency2, Dependency3>()
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

Test(ContainerBasics, getTransiantWithDirectDependency) {
    clonixin::Container &c = clonixin::defaultContainer();
    using namespace clonixin::type_desc;
    using namespace clonixin::utils::value;

    using TestClass = tt::Transient<int>;

    c.addType<Transient<TestClass>, Int<0>>();

    std::shared_ptr<TestClass> ptr = c.getInstance<TestClass>();
    cr_assert(bool(ptr), "Returned pointer should not be NULL.");
}
