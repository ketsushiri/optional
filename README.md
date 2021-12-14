# optional
Either monad implementation in C++. Just a studing project, but I really likes it.

## Interface
Defined two functions for Optional:
```c++
/* return. pure, because we can't use return here. */
template<class L, class R>
Optional<L, R> pure (const L& left, const L& right);

/* monadic computations binder, right to left. */
template<class L, class R>
Optional<L, R> operator<<= (Optional<L, R>(*arrow)(R), Optional<L, R> value);
```

In Haskell the same functions are main Monad typeclass funtions.
```haskell
class Monad m where
    return :: a -> m a -- pure in Applicative, same.
    (>>=)  :: m a -> (a -> m b) -> m b
```
Pure is simple Kleisli Id arrow realisation, but we should also provide a default left value.

## What done?
Main monad type as `Optional<L, R>` class, defined like this.
```c++
template <class L, class R>
class Optional {
  public:
    R right_value;
    L left_value;
    bool failed;

    Optional(L lvalue, R rvalue);

    void left(L value); // also set failed = true;
    void right(R value);
    bool is_failed();
};
```
So far, if failed is set on `true` then the next computations won't be performed. Otherwise we have `<<=` (>>= in Haskell), which calls a function (which is left operand) with a `right_value` field value as an argument. Binder is working as it should work, the main difference from Haskell's one is that it is right associative, so the sequence looks like this: 
```c++
Optional<std::string, int> some_function(int value)
{
    // the default values in a contructor.
    Optional<std::string, int> start (std::string("ok"), value);
    return h <<= g <<= f <<= start;
}
```
Arrows above is a functions with type `int -> Optional<std::string, int>`, or, if in general, `R -> Optional<L, R>`. For more detailed explanation check `optional.cpp` sources.

