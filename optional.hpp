
#ifndef OPTIONAL
#define OPTIONAL

template <class L, class R>
class Optional {
  public:
    L left_value;
    R right_value;
    bool failed;

    Optional(L lvalue, R rvalue);

    void left(L lvalue);
    void right(R rvalue);
    bool is_failed();
};

template <class L, class R>
Optional<L, R> pure(const L& def_err_value, const R& value);

#endif

