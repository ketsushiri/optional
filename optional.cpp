#include<iostream>

// Either monad.
// Error (left) value on left, Right on right, as it should be.
template <class L, class R>
class Optional {
  public:
    L left_value;
    R right_value;
    bool failed = false;

    Optional(const L, const R);

    void left(const L&);
    void right(const R&);
    bool is_failed();
};

// Binder operator as macro.
#define bind(L, R) == __binder<L, R>() == // shit we have general Optional type....

// Main binder class.
// Operator (==) for computations with effect.
// The left operand should be Optional<L, R> type, the right one is Kleili arrow,
// 
// In Haskell we have:
// (>>=) :: m a -> (a -> m b) -> m b
//
// Here i tried to do the same. The bind looks like this:
// Optional<L, R> bind(L, R) (R -> Optional<L, R>)

template <class L, class R>
class __binder {
  public:
    bool __failed = false;
    R right_value;
    L left_value;

    __binder() {};
};

// The right binder (Optional<L,R> == __binder)
template <class L, class R>
inline __binder<L, R>& operator==(Optional<L, R>& lvalue, const __binder<L, R>& rbinder)
{
    // Here we must declare new __binder as we changing it's fields.
    // BUT, the another approach is declare some global dummy binder.
    // The problem is, that we have general Optional<L, R>, where L, R is type templates.
    // Therefore need to think really hard, because memory leaks...
    static __binder<L, R> new_binder; // memleak btw.
    if (lvalue.is_failed()) {
        new_binder.left_value = lvalue.left_value;
        new_binder.__failed = true;
    }
    else
        new_binder.right_value = lvalue.right_value;

    return new_binder;
}

// The left binder (__binder == Optional<L,R>)
template <class L, class R>
inline Optional<L, R>& operator==(__binder<L, R>& lbinder, Optional<L, R>(*arrow)(R))
{
    // If the __failed is true, then computation sequence failed; 
    // therefore we shouldn't perform next computations.
    static Optional<L, R> result (lbinder.left_value, lbinder.right_value);
    if (lbinder.__failed)
        result.failed = true;
    else
        result = arrow(result.right_value);

    return result;
}

// Trivial Optional<L, R>'s methods realisations.
// 
// Should get some "default" values, as it is a constructor...
template <class L, class R>
Optional<L, R>::Optional(const L lvalue, const R rvalue)
{
    left_value  = lvalue;
    right_value = rvalue; 
}

template <class L, class R>
void Optional<L, R>::left(const L& lvalue)
{
    failed = true;
    left_value = lvalue;
}

template <class L, class R>
void Optional<L, R>::right(const R& rvalue)
{
    right_value = rvalue;
}

// This is like the OOP style, isn't it?
template <class L, class R>
bool Optional<L, R>::is_failed()
{
    return failed;
}

// This is also a binder, but as a function.
// There is no difference, except it looks like shit.
template <class L, class R>
Optional<L, R> binder(Optional<L, R>& value, Optional<L, R>(*arrow)(R))
{
    if (is_failed(value))
        return value;

    Optional<L, R> result = arrow(value.right_value);
    return result;
}

// Test Kleili arrow. Get's R type value, returns Optional<L, R>.
Optional<std::string, int> inc(int value)
{
    Optional<std::string, int> result (std::string("ok"), value);
    if (value > 10)
        result.left(std::string("value is more than 10!"));
    else
        result.right(value + 1);

    // Also the output just for check purpose.
    std::cout << "Left: " << result.left_value
    << "; Right: " << result.right_value << std::endl;

    return result;
}

Optional<std::string, int> result() {
    int start_value;
    std::cin >> start_value;

    // Init start value.
    Optional<std::string, int> start (std::string("ok"), start_value);

    // Here the sequence is.
    // The binder is leaking every time it's constructor is called.
    // Because the sequence is kinda like this:
    //
    // __binder() -> __binder() fields is set from left Optional
    // -> __binder() with fields passed as a left binder -> new __binder initialised as static
    // -> return refference of new __binder(), but the old one isn't destructed still.
    //
    // This is like the sequece of `Optional<L,R> bind(L,R) Arrow`
    // If we bind the result next to some Arrow2, the sequece for __binder will be the same and we will get
    //   another __binder() object in memory, on which no one points. Such cases...
    
    return ( start bind(std::string, int) inc
                   bind(std::string, int) inc
                   bind(std::string, int) inc
                   bind(std::string, int) inc );

    /*
        inc :: Int -> Either Int String
        inc x =
          case x > 10 of
            True -> Left "value is more than 10"
            False -> Right $ x + 1


        main :: Int -> Either Int String
        main = \x -> return x >>= inc >>= inc >>= inc >>= inc
            -- The same thing, but on the right language.
     */
}

signed main() {
    Optional<std::string, int> res = result();
    return 0;
}

