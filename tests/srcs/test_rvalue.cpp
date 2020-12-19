#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerRvalue, .description = "Testing container features: Retrieving rvalue references on types.");

Test(ContainerRvalue, getTransientNoDependenciesOnce, .description = "register a simple instance of a transient object and build it.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Transient<>;

    container.addType<Transient<TestClass>>();

    tt::Transient<> && val = container.getInstance<TestClass &&>();

    cr_assert_eq(TestClass::getCount(), 1, "More than one instance of TestClass was built.");
}

Test(ContainerRvalue, getTransientNoDependenciesTwoTimes, .description = "register a simple instance of a transient object and build it more than once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using TestClass = tt::Transient<>;

    container.addType<Transient<TestClass>>();

    tt::Transient<> val(std::move(container.getInstance<TestClass &&>()));
    tt::Transient<> val2 = std::move(container.getInstance<TestClass &&>());

    cr_expect_not(val == val2, "Returned pointers should refer to different objects.");
    cr_assert_eq(TestClass::getCount(), 2, "Number of TestClass instances should be 2. Built %d times(s)", TestClass::getCount());
}

Test(ContainerRvalue, getTransientWithTransientDependenciesOnce, .description = "Register several classes with dependencies and retrieve one of them once.", .disabled = false) {
    clonixin::Container container;
    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<Empty &&>;
    using Dependency3 = tt::Transient<Dependency2 &&>;
    using TestClass = tt::Transient<Dependency &&, Dependency3 &&>;

    container
        .addType<Transient<Empty>>()
        .addType<Transient<Dependency>>()
        .addType<Transient<Dependency2>, Empty &&>()
        .addType<Transient<Dependency3>, Dependency2 &&>()
        .addType<Transient<TestClass>, Dependency &&, Dependency3 &&>()
    ;

    TestClass && val = container.getInstance<TestClass &&>();

    cr_assert_eq(Dependency::getCount(), 1, "Dependency should be built exactly once. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 1, "TestClass should be built exactly once. Built %d time(s)", TestClass::getCount());
}

Test(ContainerRvalue, getTransientWithTransientDependenciesTwoTimes, .description = "Register several classes with dependencies and retrieve one of them twice.", .disabled = false) {
    clonixin::Container container;

    class Empty {};

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Transient<Empty &&>;
    using Dependency3 = tt::Transient<Dependency2 &&>;
    using TestClass = tt::Transient<Dependency &&, Dependency3 &&>;

    container
        .addType<Transient<Empty>>()
        .addType<Transient<Dependency>>()
        .addType<Transient<Dependency2>, Empty &&>()
        .addType<Transient<Dependency3>, Dependency2 &&>()
        .addType<Transient<TestClass>, Dependency &&, Dependency3 &&>()
    ;

    TestClass val = container.getInstance<TestClass &&>();
    TestClass val2 = container.getInstance<TestClass &&>();

    cr_assert_not(val == val2, "Returned value should be different (have different address).");
    cr_assert_eq(Dependency::getCount(), 2, "Dependency should be built exactly two times. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 2, "Dependency2 should be built exactly two times. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 2, "Dependency3 should be built exactly two times. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 2, "TestClass should be built exactly two times. Built %d time(s)", TestClass::getCount());
}

Test(ContainerRvalue, getTransientWithMixedDependencies, .description = "Register several classes with dependencies and retrieve one them twice.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using Dependency = tt::Transient<>;
    using Dependency2 = tt::Singleton<Dependency &&>;
    using Dependency3 = tt::Singleton<std::shared_ptr<Dependency2>>;
    using TestClass = tt::Transient<Dependency &&, std::shared_ptr<Dependency2>, std::shared_ptr<Dependency3>>;

    container
        .addType<Transient<Dependency>>()
        .addType<Singleton<Dependency2>, Dependency &&>()
        .addType<Singleton<Dependency3>, Dependency2>()
        .addType<Transient<TestClass>, Dependency &&, Dependency2, Dependency3>()
    ;

    TestClass val = container.getInstance<TestClass &&>();
    TestClass val2 = container.getInstance<TestClass &&>();

    cr_assert_not(val == val2, "Returned pointers should not be equals.");
    cr_assert_eq(Dependency::getCount(), 3, "Dependency should be built exactly three times. Built %d time(s)", Dependency::getCount());
    cr_assert_eq(Dependency2::getCount(), 1, "Dependency2 should be built exactly once. Built %d time(s)", Dependency2::getCount());
    cr_assert_eq(Dependency3::getCount(), 1, "Dependency3 should be built exactly once. Built %d time(s)", Dependency3::getCount());
    cr_assert_eq(TestClass::getCount(), 2, "TestClass should be built exactly two times. Built %d time(s)", TestClass::getCount());
}
