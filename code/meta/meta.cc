/**
 * @file meta.cc
 * @brief performs a test of primality at compile time
 *        using c++ template instantiation.
 */ 

/**
 * @class Natural
 * @brief defines a type representing all natural number.
 */
template<class V> struct Natural { typedef V value; };

/**
 * @class zero
 * @brief Axiom defining zero.
 */
struct zero : public Natural<zero> { };

/**
 * @class S
 * @brief Definition of the successor function
 */
template<class C> struct S : public Natural<S<C> > { typedef C predecessor; };

/* name integers from zero to ten */
typedef S<zero> one;
typedef S<one> two;
typedef S<two> three;
typedef S<three> four;
typedef S<four> five;
typedef S<five> six;
typedef S<six> seven;
typedef S<seven> eight;
typedef S<eight> nine;
typedef S<nine> ten;

/**
 * @class Plus
 * @brief Defines addition on natural integers.
 */
template<class C,class D> struct Plus
        : public S<Plus<C,typename D::predecessor> > { };

// specialized template when second argument is zero
template<class C> struct Plus<C,zero>
        : public C { };

/**
 * @class Minus
 * @brief Defines addition on natural integers.
 */
template<class C,class D> struct Minus
        : public Minus<C,typename D::predecessor>::predecessor { };

// specialized template when second argument is zero
template<class C> struct Minus<C,zero>
        : public C { };

/**
 * @class Times
 * @brief Defines multiplication on natural integers.
 */
template<class C,class D> struct Times
        : public Plus<C,typename Times<C,typename D::predecessor>::value> { };

// specialized template when second argument is zero
template<class C> struct Times<C,zero>
        : public zero { };

/**
 * @class Hundred
 * @brief Defines multiplication on natural integers.
 */
// define one Hundred
typedef Times<ten,ten> Hundred;

/**
 * @class ge
 * @brief Defines the "greater or equal" test
 */
template<class C,class D> struct ge
        : public ge<typename C::predecessor,typename D::predecessor> { };
// specialized template when second argument is zero
template<class C> struct ge<C,zero>
        : public one { };
// specialized template when first argument is zero
template<class C> struct ge<zero,C>
        : public zero { };
// specialized template when both arguments are zero
template<> struct ge<zero,zero>
        : public one { };

/**
 * @class IsDivibleBy
 * @brief Defines if first template argument C can be devided by
 * second template argument D. Third template argument tells if
 * C<D.
 */

// specialized template when 3rd argument is 2, that is,
// we did not compute yet if C<D or C>=D
template<class C,class D,class E = S<S<zero> > > struct IsDivibleBy { };
// specialized template when 3rd argument is 2, to fill value
// telling if C<D or C>=D
template<class C,class D> struct IsDivibleBy<C,D,S<S<zero> > >
        : public IsDivibleBy<C,D,typename ge<C,D>::value> { };
// specialized template when C<D and C is not zero
// in this case D does not devide C
template<class C,class D> struct IsDivibleBy<C,D,zero>
        : public zero { };
// specialized template when C<D and C is not zero
// in this case D devides C, so inherit one
template<class C> struct IsDivibleBy<zero,C,zero>
        : public one { };
// specialized template when C>=D:
// D divides C if and only if D divides C-D.
template<class C,class D> struct IsDivibleBy<C,D,S<zero> >
        : public IsDivibleBy<typename Minus<C,D>::value,D> { };

/**
 * @class IsPrime
 * @brief Defines if a natural number is prime.
 *
 * Template argument C: C is the number to test for primality.
 * Template argument D: Currently trying to see if D devides C.
 * Template argument S: Tells if we reached the end and there is no divisor.
 * Template argument E: Stores if C is divisible by D.
 * Template argument F: Stores if D>=C.
 */
// Primality test
template<class C,
         class D = two,
         class S = zero,
         class E = zero,
         class F = zero> struct IsPrime { };

// We perform a loop from two to the integer to test minus 1

// specialized template when we are at the end
template<class C,class D> struct IsPrime<C,D,zero,zero,zero>
        : public IsPrime<C,D,one,zero,typename ge<D,C>::value> { };

// specialized template to explore number after D
// in case there is no divisibility.
template<class C,class D> struct IsPrime<C,D,one,zero,zero>
        : public IsPrime<C,S<D>,zero,typename IsDivibleBy<C,D>::value,zero> { };

// specialized template to return zero if there is divisibility.
template<class C,class D> struct IsPrime<C,D,zero,one,zero>
        : public zero { };

// specialized template when we reach the end of the loop without any divisor.
template<class C,class D> struct IsPrime<C,D,one,zero,one>
        : public one { };
/**
 * @class Decimal2
 * @brief Write a number with two digits in base 10
 */
template<class C,class D> struct Decimal2
        : public Plus<typename Times<ten,C>::value,D> { };

/**
 * @class Decimal3
 * @brief Write a number with three digits in base 10
 */
template<class C,class D,class E> struct Decimal3
    : public Plus<Plus<typename Times<Hundred,C>::value,
                       typename Times<ten,D>::value>,E>::value { };

//
#include <iostream>
using namespace std;
template<class C> string output(C);
template<> string output(zero) { return "is not prime"; }
template<> string output(one) { return "is a prime number"; }

int main() {
    // is 97 prime?
    cout << "97 "
         << output(IsPrime<Decimal2<nine,seven>::value>::value()) << endl;
    // is 99 prime?
    cout << "99 "
         << output(IsPrime<Decimal2<nine,nine>::value>::value()) << endl;
    // is 443 prime?
    // requires "-ftemplate-depth=1237" for compilation with g++
    //cout << "443 "
    //     << output(IsPrime<Decimal3<four,four,three>::value>::value()) << endl;
    return 0;
}

