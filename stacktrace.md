This page tells how to display a demangled stack trace for a c++ program under linux.

We used the ```backtrace``` and ```backtrace_symbols``` system calls.

Then command line tool ```addr2line```, and the ```abi::__cxa_demangle``` function to display human readable version.

![code is here](https://github.com/fderepas/unix_memo/blob/main/code/stracktrace/stacktace.cc)

Here is a result of execution:

```
Stack trace:
in printStackTrace()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:15
in f1()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:58
in f2()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:59
in f3()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:60
in f4()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:61
in f5()
    at /home/fabrice/git/unix_memo/code/stacktrace/stacktrace.cc:62
```


