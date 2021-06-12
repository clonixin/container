#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerDuplicateManual, .description = "Testing container behavior when re-registering a type.", .disabled = false);

Test(ContainerDuplicateManual, addTransientTwiceNoTag, .description = "register a transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(tt::Transient<>) {}
    };

    container.addTransient(std::make_unique<GenericBuilder<TestClass>>());
    container.addTransient(std::make_unique<GenericBuilder<tt::Transient<>>>());
    container.addTransient(std::make_unique<GenericBuilder<TestClass, tt::Transient<> &&>>());

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addAbstractTransientTwiceNoTag, .description = "register an abstract transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass0>>());
    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass1>>());

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addSingletonTwiceNoTag, .description = "register a transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addSingleton(std::make_unique<GenericBuilder<TestClass>>());
    container.addSingleton(std::make_unique<GenericBuilder<tt::Singleton<>>>());
    container.addSingleton(std::make_unique<GenericBuilder<TestClass, tt::Singleton<>>>());

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addInstanceTwiceNoTag, .description = "register a transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass(int i): _i(i) {}
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
            int _i;
    };

    int SUCCESS = 1;
    int FAIL = 0;

    container.addInstance(std::make_unique<TestClass>(FAIL));
    container.addInstance(std::make_unique<TestClass>(SUCCESS));

    auto tc = container.getInstance<TestClass>();
    cr_assert_eq(tc->_i, SUCCESS, "Wrong instance was kept in container.");
}

Test(ContainerDuplicateManual, addAbstractSingletonTwiceNoTag, .description = "register an abstract transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass0>>());
    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass1>>());

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addTransientTwiceOverride, .description = "register a transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(tt::Transient<>) {}
    };

    container.addTransient(std::make_unique<GenericBuilder<TestClass>>());
    container.addTransient(std::make_unique<GenericBuilder<tt::Transient<>>>());
    container.addTransient(std::make_unique<GenericBuilder<TestClass, tt::Transient<> &&>>(), over);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addAbstractTransientTwiceOverride, .description = "register an abstract transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass0>>());
    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass1>>(), over);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addSingletonTwiceOverride, .description = "register a transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addSingleton(std::make_unique<GenericBuilder<TestClass>>());
    container.addSingleton(std::make_unique<GenericBuilder<tt::Singleton<>>>());
    container.addSingleton(std::make_unique<GenericBuilder<TestClass, tt::Singleton<>>>(), over);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addInstanceTwiceOverride, .description = "register a transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass(int i): _i(i) {}
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
            int _i;
    };

    int SUCCESS = 1;
    int FAIL = 0;

    container.addInstance(std::make_unique<TestClass>(FAIL), over);
    container.addInstance(std::make_unique<TestClass>(SUCCESS), over);

    auto tc = container.getInstance<TestClass>();
    cr_assert_eq(tc->_i, SUCCESS, "Wrong instance was kept in container.");
}

Test(ContainerDuplicateManual, addAbstractSingletonTwiceOverride, .description = "register an abstract transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass0>>());
    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass1>>(), over);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addTransientTwiceIgnore, .description = "register a transient type twice. Explicit ignore.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(tt::Transient<>) {}
    };

    container.addTransient(std::make_unique<GenericBuilder<tt::Transient<>>>());
    container.addTransient(std::make_unique<GenericBuilder<TestClass, tt::Transient<> &&>>(), ignore);
    container.addTransient(std::make_unique<GenericBuilder<TestClass>>(), ignore);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addAbstractTransientTwiceIgnore, .description = "register an abstract transient type twice. Explicit ignore dupe.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass1>>());
    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass0>>(), ignore);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addSingletonTwiceIgnore, .description = "register a transient type twice. Explicit ignore dupes.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addSingleton(std::make_unique<GenericBuilder<tt::Singleton<>>>());
    container.addSingleton(std::make_unique<GenericBuilder<TestClass, tt::Singleton<>>>());
    container.addSingleton(std::make_unique<GenericBuilder<TestClass>>(), ignore);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addInstanceTwiceIgnore, .description = "register a transient type twice. Explicit ignore dupes.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass(int i): _i(i) {}
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
            int _i;
    };

    int SUCCESS = 1;
    int FAIL = 0;

    container.addInstance(std::make_unique<TestClass>(SUCCESS), ignore);
    container.addInstance(std::make_unique<TestClass>(FAIL), ignore);

    auto tc = container.getInstance<TestClass>();
    cr_assert_eq(tc->_i, SUCCESS, "Wrong instance was kept in container.");
}

Test(ContainerDuplicateManual, addAbstractSingletonTwiceIgnore, .description = "register an abstract transient type twice. Explicit ignore.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass1>>());
    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass0>>(), ignore);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addTransientTwiceOnce, .description = "register a transient type twice. Explicit once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(tt::Transient<>) {}
    };

    container.addTransient(std::make_unique<GenericBuilder<tt::Transient<>>>());
    container.addTransient(std::make_unique<GenericBuilder<TestClass, tt::Transient<> &&>>(), once);
    cr_assert_throw(container.addTransient(std::make_unique<GenericBuilder<TestClass>>(), once), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addAbstractTransientTwiceOnce, .description = "register an abstract transient type twice. Explicit once dupe.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not onced"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass1>>());
    cr_assert_throw((container.addTransient(std::make_unique<AbstractBuilder<Interface, TestClass0>>(), once)), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicateManual, addSingletonTwiceOnce, .description = "register a transient type twice. Explicit once dupes.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not onced"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addSingleton(std::make_unique<GenericBuilder<tt::Singleton<>>>());
    container.addSingleton(std::make_unique<GenericBuilder<TestClass, tt::Singleton<>>>());
    cr_assert_throw(container.addSingleton(std::make_unique<GenericBuilder<TestClass>>(), once), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addInstanceTwiceOnce, .description = "register a transient type twice. Explicit once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class TestClass {
        public:
            TestClass(int i): _i(i) {}
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
            int _i;
    };

    int SUCCESS = 1;
    int FAIL = 0;

    container.addInstance(std::make_unique<TestClass>(FAIL), once);
    cr_assert_throw((container.addInstance(std::make_unique<TestClass>(SUCCESS), once)), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicateManual, addAbstractSingletonTwiceOnce, .description = "register an abstract transient type twice. Explicit once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;
    using namespace clonixin::builders;

    class Interface {
        public:
            virtual ~Interface() {}

            virtual void stuff() = 0;
    };

    class TestClass0 : public Interface {
        public:
            TestClass0() { cr_assert_fail("Wrong ctor called. The factory was not onced"); }

            void stuff() override {}
    };

    class TestClass1 : public Interface {
        public:
            TestClass1() {}

            void stuff() override {}
    };

    container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass1>>(), once);
    cr_assert_throw((container.addSingleton(std::make_unique<AbstractBuilder<Interface, TestClass0>>(), once)), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<Interface>();
}

