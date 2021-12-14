/*
    Simple C++ Either monad implementation.
    
    Main template class - Optional<L, R>.
    Fail value and it's type on the left, right value and it's type on the right.

    Monadic binder - overloaded '<<=' operator, which is right associative.
        So the sequence will look like this:
            `arrow2 <<= arrow1 <<= value`

    The first right value should be type Optional<L, R>.
    The arrows should be Kleisli arrows for Either monad, as it is defined in Haskell:
        general monad binder: (>>=) :: Monad m => m a -> (a -> m b) -> m b
        monad binder for (Either e) :: Either e a -> (a -> Either e b) -> Either e b

    If any single computation in a sequence is failed, then the next computations won't be performed.
 */

#include<iostream>

// Inline flag if we are not exporting anything.
#define INLINING

// * not Either, but Optional.
template <class L, class R>
class Optional {
  public:
    L left_value;
    R right_value;
    bool failed; // is true, then next computations won't be performed.

    Optional(L lvalue, R rvalue);

    void left(L lvalue);
    void right(R rvalue);
    bool is_failed();
};

// * main bind operator. Right to left.
//  ex: `arrow3 <<= arrow2 <<= arrow1 <<= value`
template <class L, class R>
#ifdef INLINING
inline
#endif
Optional<L, R> operator<<= (Optional<L, R>(*arrow)(R), Optional<L, R> value)
{
    return (value.is_failed())
        ? value
        : arrow(value.right_value);
}

// Trivial monad's methods realisation.
template <class L, class R>
#ifdef INLINING
inline
#endif
Optional<L, R>::Optional(L lvalue, R rvalue)
{
    left_value  = lvalue;
    right_value = rvalue;
    failed      = false;
}

template <class L, class R>
#ifdef INLINING
inline
#endif
void Optional<L, R>::left(L lvalue)
{
    failed     = true;
    left_value = lvalue; 
}

template <class L, class R>
#ifdef INLINING
inline
#endif
void Optional<L, R>::right(R rvalue)
{
    right_value = rvalue;
}

// * the OOP style method for fields access, I guess...
template <class L, class R>
#ifdef INLINING
inline
#endif
bool Optional<L, R>::is_failed()
{
    return failed;
}

// Usage examples
//
// * Kleisli arrow.
inline Optional<std::string, int> inc(int value)
{
    Optional<std::string, int> result (std::string("ok"), value);
    if (value > 10)
        result.left(std::string("value is more than 10!"));
    else
        result.right(value + 1);

    std::cout << "Left: " << result.left_value
    << "; Right: " << result.right_value << std::endl;
    return result;
}

// * simple test usage.
// * bind opeartor is right to left.
inline Optional<std::string, int> run_test()
{
    int start_value;
    std::cin >> start_value;
    Optional<std::string, int> start (std::string("ok"), start_value);

    return inc <<= inc <<= inc <<= start;
}

int main(int argc, char** argv)
{
    auto result = run_test();
    std::cout << "Result left: " << result.left_value
        << "; result right: " << result.right_value << std::endl;
    return 0;
}

