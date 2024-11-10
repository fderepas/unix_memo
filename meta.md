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

