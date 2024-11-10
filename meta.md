# Meta programming in C++.

We are going to perform a test of primality at compile time, using C++ template instantiation mechanism.

![code is here](https://github.com/fderepas/unix_memo/tree/main/code/meta)

Let's define the set of natural integers with:

```c++
template<class V> struct Natural { typedef V value; };
```
Then we define zero as:
```c++
struct zero : public Natural<zero> { };
```
and the sucessor of an integer with type ```S```:
```c++
template<class C> struct S : public Natural<S<C> > { typedef C predecessor; };
```
We can see that type ```S``` define a type named ```predecessor``` representing the type on which ```S``` was applied.

Then ```one``` can be defined as:
```c++
typedef S<zero> one;
```

Then all numbers up to 10:
```c++
typedef S<one> two;
typedef S<two> three;
typedef S<three> four;
typedef S<four> five;
typedef S<five> six;
typedef S<six> seven;
typedef S<seven> eight;
typedef S<eight> nine;
typedef S<nine> ten;
```

Then addition on natural integers can be defined with the following type ```Plus```:
```c++
template<class C,class D> struct Plus
        : public S<Plus<C,typename D::predecessor> > { };
        
// specialized template when second argument is zero and has
// no predecessor type defined:
template<class C> struct Plus<C,zero>
        : public C { };
```
In the same way we define substraction on natural integers:
```c++
template<class C,class D> struct Minus
        : public Minus<C,typename D::predecessor>::predecessor { };
// specialized template when second argument is zero
template<class C> struct Minus<C,zero>
        : public C { };
```
Since we only have positive integer one can notice that ```Minus<one,two>``` can not be computed. It's going to be transformed into ```Minus<one,zero>::predecessor::predecessor```, which is ```one::predecessor::predecessor``` which does not exist.

We can define multiplication:
```c++
template<class C,class D> struct Times
        : public Plus<C,typename Times<C,typename D::predecessor>::value> { };
// specialized template when second argument is zero
template<class C> struct Times<C,zero>
        : public zero { };
```

Now let's map boolean values to types ```zero``` for false and ```one``` for true. Using this, we define "greater or equal", being a type which is ```zero``` if C is greater or equal to D and ```one``` otherwise:
```c++
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
```

Now let's defines if first template argument C can be devided by
second template argument D. Third template argument tells if C<D.
```c++
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
```

We can now implement a primality test. Here is the type to implement that:
```c+
template<class C,
         class D = two,
         class S = zero,
         class E = zero,
         class F = zero> struct IsPrime { };
```
Here is the meaning of each template argument:
 * Template argument C: C is the number to test for primality.
 * Template argument D: Currently trying to see if D devides C.
 * Template argument S: Tells if we reached the end and there is no divisor.
 * Template argument E: Stores if C is divisible by D.
 * Template argument F: Stores if D>=C.

Then we are going to perform a loop from two to the integer to test minus 1.

```c++
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
        : public one { }
```
