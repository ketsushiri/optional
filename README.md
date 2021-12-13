# optional
Either monad implementation in C++. Just a studing project, but I really likes it.

## What done?
Main monad type as `Optional<L, R>` class, defined like this.
```c++
template <class L, class R>
class Optional {
  public:
    R right_value;
    L left_value;
    bool failed;
};
```
So far if failed is set on `true` then the next computations won't be performed. Otherwise we have `bind` (>>= in Haskell), which calls a function (which is right operand) with a `right_value` field value as an argument. Binder is working as it should work, but, because we have general type and the c-preprocessor don't know anything about types, we should call `bind` operator with an arguments (which is L and R types).
```c++
Optional<std::string, int> some_function(int value)
{
    Optional<std::string, int> start (std::string("ok"), value); // the default values in contructor.
    return ( start bind(std::string, int) arrow bind(std::string, int) arrow ); // ... 
}
```
Arrow above is function with type `int -> Optional<std::string, int>`, or, in general, `R -> Optional<L, R>`.

Oh, and there is also a memory leaks in `__binder` class, because it's too general. There is an approach to fix it, but it will become not so general... I don't like C++. Or C++ do not likes me... Anyway, check `optional.cpp` for more info. 
