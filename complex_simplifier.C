
/*
 * This uses template expressions to symbolically eliminate
 * zeros from complex arithmetic expressions. Because there are
 * many zero coefficients in the Humlicek rationals, this gets
 * rid of some zero operations, maybe!
 */

#include <vector>
#include <iostream>
#include <cstddef>
#include <cassert>
#include <string>

template <typename E>
class ComplexExpression {
  public:
    std::string re() const 
    {
        return static_cast<E const&>(*this).real();
    }
    std::string im() const 
    {
        return static_cast<E const&>(*this).imag();
    }

    bool imag_zero() const
    {
        return static_cast<E const&>(*this).imag_zero();
    }
    bool real_zero() const
    {
        return static_cast<E const&>(*this).real_zero();
    }
};

class ComplexVariable : public ComplexExpression<ComplexVariable> {

  std::string name_;
  bool re_zero_ {false};
  bool im_zero_ {false};

private:
  const std::string real_text = "_r";
  const std::string imag_text = "_i";

public:
  ComplexVariable(std::string const& name, bool re_zero, bool im_zero) :
    name_(name), re_zero_(re_zero), im_zero_(im_zero) {}

  std::string real() const {
    return name_ + real_text;
  }
  std::string imag() const {
    return name_ + imag_text;
  }

  bool real_zero() const { return re_zero_; }
  bool imag_zero() const { return im_zero_; }
};

template <typename E1, typename E2>
class ComplexSum : public ComplexExpression<ComplexSum<E1, E2>> {
  E1 const _u;
  E2 const _v;
private:
  const std::string plus = " + ";
  const std::string lparen = "(";
  const std::string rparen = ")";
public:
  ComplexSum(E1 const& u, E2 const& v) : _u(u), _v(v) {}

  std::string real() const {
    if (_u.real_zero() && _v.real_zero()) return "0.0";
    else if (_u.real_zero()) return _v.real();
    else if (_v.real_zero()) return _u.real();
    return lparen + _u.real() + plus + _v.real() + rparen;
  }

  std::string imag() const {
    if (_u.imag_zero() && _v.imag_zero()) return "0.0";
    else if (_u.imag_zero()) return _v.imag();
    else if (_v.imag_zero()) return _u.imag();
    return lparen + _u.imag() + plus + _v.imag() + rparen;
  }

  bool real_zero() const {
    if (_u.real_zero() && _v.real_zero()) return true;
    return false;
  }
  bool imag_zero() const {
    if (_u.imag_zero() && _v.imag_zero()) return true;
    return false;
  }
};

template <typename E1, typename E2>
class ComplexTimes : public ComplexExpression<ComplexTimes<E1, E2>> {
  E1 const _u;
  E2 const _v;
private:
  const std::string times = " * ";
  const std::string minus = " - ";
  const std::string plus = " + ";
  const std::string lparen = "(";
  const std::string rparen = ")";
public:
  ComplexTimes(E1 const& u, E2 const& v) : _u(u), _v(v) {}

  std::string real() const {
    if (_u.imag_zero() || _v.imag_zero())
      return lparen + _u.real() + times + _v.real() + rparen;
    else if (_u.real_zero() || _v.real_zero())
      return lparen + minus + _u.imag() + times + _v.imag() + rparen;
    else
      return lparen + _u.real() + times + _v.real() + minus + _u.imag()
        + times + _v.imag() + rparen;
  }

  std::string imag() const {
    if (_u.real_zero() || _v.imag_zero())
      return lparen + _u.imag() + times + _v.real() + rparen;
    else if (_u.imag_zero() || _v.real_zero())
      return lparen + _u.real() + times + _v.imag() + rparen;
    else
      return lparen + _u.real() + times + _v.imag() + plus + _u.imag()
        + times + _v.real() + rparen;
  }

  bool real_zero() const {
    if ((_u.real_zero() || _v.real_zero()) &&
        (_u.imag_zero() || _v.imag_zero())) return true;
    return false;
  }
  bool imag_zero() const {
    if ((_u.real_zero() || _v.real_zero()) &&
        (_u.imag_zero() || _v.imag_zero())) return true;
    return false;
  }
};

// Syntactic sugarizing
template<typename E1, typename E2>
ComplexSum<E1, E2> operator+(ComplexExpression<E1> const& u, ComplexExpression<E2> const& v) {
  return ComplexSum<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}
template<typename E1, typename E2>
ComplexTimes<E1, E2> operator*(ComplexExpression<E1> const& u, ComplexExpression<E2> const& v) {
  return ComplexTimes<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}

int main() {
  // The real run..
  // const comp aa0(11.7559071436993, 0.0);
  // const comp aa1(0.0, -32.310199761603);
  // const comp aa2(-21.9357456686406, 0.0);
  // const comp aa3(0.0, 31.490536152863);
  // const comp aa4(6.75847413957232, 0.0);
  // const comp aa5(0.0, -8.07354660639634);
  // const comp aa6(-0.507771291744591, 0.0);
  // const comp aa7(0.0, 0.564189504758109);
  // const comp bb0(6.5625, 0.0);
  // const comp bb1(-52.5, 0.0);
  // const comp bb2(52.5, 0.0);
  // const comp bb3(-14.0, 0.0);

  ComplexVariable z("z", false, false);
  ComplexVariable zz("zz", false, false);

  ComplexVariable aa0("aa0", false, true);
  ComplexVariable aa1("aa1", true, false);
  ComplexVariable aa2("aa2", false, true);
  ComplexVariable aa3("aa3", true, false);
  ComplexVariable aa4("aa4", false, true);
  ComplexVariable aa5("aa5", true, false);
  ComplexVariable aa6("aa6", false, true);
  ComplexVariable aa7("aa7", true, false);

  ComplexVariable bb0("bb0", false, true);
  ComplexVariable bb1("bb1", false, true);
  ComplexVariable bb2("bb2", false, true);
  ComplexVariable bb3("bb3", false, true);

  auto num = (((((((aa7 * z + aa6) * z + aa5) * z + aa4) * z + aa3) *
               z +
             aa2) *
              z +
            aa1) *
             z +
           aa0);
  auto den = ((((zz + bb3) * zz + bb2) * zz + bb1) * zz + bb0);

  std::cout << "Re(numerator)" << std::endl;
  std::cout << den.real() << std::endl;
  std::cout << "Im(numerator)" << std::endl;
  std::cout << den.imag() << std::endl;
}
