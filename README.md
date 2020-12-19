# Clonixin Dependency Injection
Easy to use Dependency Injection Container, for c++17 and above.

## Features
### Building types
The Container accept either factories for specific types, instances, or can create a factory at compile time, to build
the type on demand.

When an instance is requested, the container will call it's factory, passing itself as a parameter to each, so these can
in turn request necessary types to the container.

If using factory, you can either pass an instance of IBuilder, which will contains the factory function, as well as a
way to retrieve the type of the object being built, or use one of the convenience functions templated to automatically
register the type, effectively wrapping a lambda or any std::function in a IBuilder instance.

For automatic builder creation, you have to specify the lifetime of the type, as well as the types of it's parameters.
The created builder will then be created on the fly, at compile time, by first checking if a constructor exist, then
creating a function call to said constructor, and passing to it unpacked direct or indirect value holding types (more on
that later), or requesting the needed instance to the Container.

### Lifetime
Upon registration of a type, a lifetime must be provided. This change the amount of created instance.

Currently, only two lifetime values are supported:
 - _Transient_ - This is the "default" lifetime. A new instance is created every time it is needed.
 - _Singleton_ - With this lifetime, a new instance is created the first time it is needed, then the same instance is
   returned each time it's requested.

More lifetimes are planned.

### Special Types

As every types are built on the fly, some conditions must be met. They either need a default constructor, or all of
their parameters should be constructible by the IoC container. As such, there is no support for arithmetic types, nor
for most standard types, apart from using the default constructor. One way to use these as an argument to pass to an
instance constructor would be to a custom IBuilder.

This means that for every types needing either an arithmetic, or special values (for example, the name of a
configuration file), the user would be required to provide a builder, which,
considering the fact that a DI container goal is to remove a lot of boilerplate code, would be against it's philosophy.

Thus were created two special types: Direct and Indirect value-holding types.

Direct types hold their value as a constexpr, and are templated on it. This means that by using Int<10>
during argument type declaration, the container will provide an int, with the value 10.

However, at the time of writing this, it is not possible, to pass some types that way:
 - Floating point types cannot be used as a template argument
 - Literal c-style string cannot be used either, due to internal linkage.
 - Literal std::string should work in C++20 **(as string can be constexpr), but as it's still barely supported, support
   for it will be added later.

    Thus came Indirect types. They use a class or structure to hold their value, providing external linkage, and
enabling to pass otherwise unsupported value.

## Getting Started

To use the container, just instantiate it, add types, and get an instance.

```c++
    #include <container.hpp>
    [...]
    int main() {
        clonixin::Container c; // or use clonixin::defaultContainer()

        using namespace clonixin::type_desc

        c
            .addType<Transient<T1>, T2>();
            .addType<Singleton<T2>>()
        ;

        std::shared_ptr<T1> inst = c.getInstance<T1>();
        inst->do_stuff()
    }
```

If you'd like to retrieve a type as an instance of it's base class, you just have to precise it.

```c++
    #include <container.hpp>
    [...]
    int main() {
        clonixin::Container c; // or use clonixin::defaultContainer()

        using namespace clonixin::type_desc

        c
            .addType<Transient<Interface, T1>, T2>();
            .addType<Singleton<T2>>()
        ;

        std::shared_ptr<Interface> inst = c.getInstance<Interface>();
        inst->do_stuff()
    }
```

If you'd rather retrieve an instance by using move semantic,
it can be done for transient, non-polymorphic types
```c++
    #include <container.hpp>
    [...]
    int main() {
        clonixin::Container c; // or use clonixin::defaultContainer()

        using namespace clonixin::type_desc

        c
            .addType<Transient<T1>, T2>();
            .addType<Transient<T3>, T1 &&>(); //T3 will be built using move semantics to retrieve a value of T1, instead of a pointer.
            .addType<Singleton<T2>>()
        ;

        T1 inst = c.getInstance<T1 &&>(); // move constructor is called if available.
        T2 inst = c.getInstance<T2 &&>(); // throws an exception, as copying or moving a singleton makes no sense.
        // the previous line won't even compile if copy and move operation are deleted.
        inst.do_stuff()
    }
```

## Planned Features
- A proper wiki
- Cached Lifetime: Act as a Singleton, but is removed if no more references to it exists.

