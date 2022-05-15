/*
    Simple C++ Either monad implementation.
    If any computation in the sequence is failed, then next one won't be performed.
*/

#include<iostream>
#include"optional.hpp"

/* Monadic interface. */

/* Binder, but from right to left.
   Sequence e.g.: arrow3 <<= arrow2 <<= arrow1 <<= value */
template <class L, class R>
Optional<L, R> operator<<= (Optional<L, R>(*arrow)(R), Optional<L, R> value)
{
    return (value.is_failed())
        ? value
        : arrow(value.right_value);
}

/* 'pure' instead of 'return'.
   Default left value should be passed in a constructor. */
template <class L, class R>
Optional<L, R> pure(const L& def_err_value, const R& value)
{
    return Optional<L, R> (def_err_value, value);
}

template <class L, class R>
Optional<L, R>::Optional(L lvalue, R rvalue)
{
    left_value  = lvalue;
    right_value = rvalue;
    failed      = false;
}

template <class L, class R>
void Optional<L, R>::left(L lvalue)
{
    failed     = true;
    left_value = lvalue; 
}

template <class L, class R>
void Optional<L, R>::right(R rvalue)
{
    right_value = rvalue;
}

template <class L, class R>
bool Optional<L, R>::is_failed()
{
    return failed;
}

/* Usage examples */

// Kleisli arrow. :: (int -> m int)
inline Optional<std::string, int> inc(int value)
{
    auto result = pure(std::string("ok"), value);
    (value > 10)
        ? result.left(std::string("value is more than 10!"))
        : result.right(value + 1);

    std::cout << "Left: " << result.left_value
    << "; Right: " << result.right_value << std::endl;
    return result;
}

// Simple example.
inline Optional<std::string, int> run_test()
{
    int start_value;
    std::cin >> start_value;
    auto start = pure(std::string("ok"), start_value);

    return inc <<= inc <<= inc <<= start;
}

int main(int argc, char** argv)
{
    auto result = run_test();
    std::cout << "Result left: " << result.left_value
              << "; result right: " << result.right_value
              << std::endl;
    return 0;
}

