This page tells how to display a demangled stack trace for a c++ program under linux.

We used the ```backtrace``` and ```backtrace_symbols``` system calls.

Then command line tool ```addr2line```, and the ```abi::__cxa_demangle``` function to display human readable version.

![code is here](https://github.com/fderepas/unix_memo/tree/main/code/stacktrace)

Here is a result of execution:

```
Stack trace:
in printStackTrace()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:15
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in A::A(int)
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
```


