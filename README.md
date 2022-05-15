# optional
Haskell Either monad implementation in C++. Just for fun.

## Interface
General monadic interface have two main methods:
```haskell
class Monad m where
    return    :: a -> m a                  -- (1)
    (>>=) m k :: m a -> (a -> m b) -> m b  -- (2)
```

(1) is a `pure` function in optional.cpp; (2) is an `<<=` operator overloaded. Binder is right to left, so it's equal to an `=<<` operator in Haskell.

When calling a constructor of `Optional<L, R>` class, default null left value should be passed also. Also can initialised with pure:

```cpp
auto value = pure(std::string("ok"), 10) // :: Optional<std::string, int>
```

## License
MIT
