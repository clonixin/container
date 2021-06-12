#include <criterion/criterion.h>

#include "./test_types.hpp"

#include "container.hpp"

namespace tt = tests::types;

TestSuite(ContainerDuplicate, .description = "Testing container behavior when re-registering a type.", .disabled = false);

Test(ContainerDuplicate, addTransientTwiceNoTag, .description = "register a transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(tt::Transient<>) {}
    };

    container.addType<Transient<TestClass>>();
    container.addType<Transient<tt::Transient<>>>();
    container.addType<Transient<TestClass>, tt::Transient<> &&>();

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractTransientTwiceNoTag, .description = "register an abstract transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;

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

    container.addType<Transient<Interface, TestClass0>>();
    container.addType<Transient<Interface, TestClass1>>();

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addSingletonTwiceNoTag, .description = "register a transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addType<Singleton<TestClass>>();
    container.addType<Singleton<tt::Singleton<>>>();
    container.addType<Singleton<TestClass>, tt::Singleton<>>();

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractSingletonTwiceNoTag, .description = "register an abstract transient type twice, without specifying behavior.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;

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

    container.addType<Singleton<Interface, TestClass0>>();
    container.addType<Singleton<Interface, TestClass1>>();

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addTransientTwiceOverride, .description = "register a transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(tt::Transient<>) {}
    };

    container.addType<Transient<TestClass>>();
    container.addType<Transient<tt::Transient<>>>();
    container.addType<Transient<TestClass>, tt::Transient<> &&>(over);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractTransientTwiceOverride, .description = "register an abstract transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Transient<Interface, TestClass0>>();
    container.addType<Transient<Interface, TestClass1>>(over);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addSingletonTwiceOverride, .description = "register a transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not overriden"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addType<Singleton<TestClass>>();
    container.addType<Singleton<tt::Singleton<>>>();
    container.addType<Singleton<TestClass>, tt::Singleton<>>(over);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractSingletonTwiceOverride, .description = "register an abstract transient type twice. Explicit override.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Singleton<Interface, TestClass0>>();
    container.addType<Singleton<Interface, TestClass1>>(over);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addTransientTwiceIgnore, .description = "register a transient type twice. Explicit ignore.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(tt::Transient<>) {}
    };

    container.addType<Transient<tt::Transient<>>>();
    container.addType<Transient<TestClass>, tt::Transient<> &&>(ignore);
    container.addType<Transient<TestClass>>(ignore);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractTransientTwiceIgnore, .description = "register an abstract transient type twice. Explicit ignore dupe.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Transient<Interface, TestClass1>>();
    container.addType<Transient<Interface, TestClass0>>(ignore);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addSingletonTwiceIgnore, .description = "register a transient type twice. Explicit ignore dupes.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addType<Singleton<tt::Singleton<>>>();
    container.addType<Singleton<TestClass>, tt::Singleton<>>(ignore);
    container.addType<Singleton<TestClass>>(ignore);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractSingletonTwiceIgnore, .description = "register an abstract transient type twice. Explicit ignore.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Singleton<Interface, TestClass1>>();
    container.addType<Singleton<Interface, TestClass0>>(ignore);

    auto tc = container.getInstance<Interface>();
}


Test(ContainerDuplicate, addTransientTwiceOnce, .description = "register a transient type twice. Explicit once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not ignored"); }
            TestClass(tt::Transient<>) {}
    };

    container.addType<Transient<tt::Transient<>>>();
    container.addType<Transient<TestClass>, tt::Transient<> &&>();
    cr_assert_throw(container.addType<Transient<TestClass>>(once), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractTransientTwiceOnce, .description = "register an abstract transient type twice. Explicit once dupe.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Transient<Interface, TestClass1>>();
    cr_assert_throw((container.addType<Transient<Interface, TestClass0>>(once)), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<Interface>();
}

Test(ContainerDuplicate, addSingletonTwiceOnce, .description = "register a transient type twice. Explicit once dupes.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

    class TestClass {
        public:
            TestClass() { cr_assert_fail("Wrong ctor called. The factory was not onced"); }
            TestClass(std::shared_ptr<tt::Singleton<>>) {}
    };

    container.addType<Singleton<tt::Singleton<>>>();
    container.addType<Singleton<TestClass>, tt::Singleton<>>(once);
    cr_assert_throw(container.addType<Singleton<TestClass>>(once), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<TestClass>();
}

Test(ContainerDuplicate, addAbstractSingletonTwiceOnce, .description = "register an abstract transient type twice. Explicit once.", .disabled = false) {
    clonixin::Container container;

    using namespace clonixin::type_desc;
    using namespace clonixin::tag::duplicate;

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

    container.addType<Singleton<Interface, TestClass1>>();
    cr_assert_throw((container.addType<Singleton<Interface, TestClass0>>(once)), clonixin::exceptions::ContainerException<clonixin::exceptions::ContainerError::DuplicateType>);

    auto tc = container.getInstance<Interface>();
}

