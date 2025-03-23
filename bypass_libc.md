This page presents how to bypass the c-library.

The 64-bit system call numbers and entry vectors is given by [https://github.com/torvalds/linux/blob/v6.7/arch/x86/entry/syscalls/syscall_64.tbl](syscall_64.tbl).

It can be found in the lib-c or in [https://docs.rs/syscalls/0.6.18/src/syscalls/arch/x86_64.rs.html](Rust Syscall) crate.

Here is an 'hello world' program in C with direct Linux system calls:
```c
// compute the length of a string for strings smaller than 100 characters
int my_strlen(const char * s) {
    int answer=0;
    while(s[answer] && answer<100)
        ++answer;
    return answer;
}

int main() {
    // message to write
    const char *message = "hello, world!\n";
    // length of the message to send
    int message_length = my_strlen(message);
    // file descriptor to write to (1=stdout)
    int file_descriptor=1;
    // number of the system call
    int system_call_number=1;
    __asm__ volatile
        ("mov %0, %%eax\n" // system call number
         "mov %1, %%edi\n" // file descriptor: stdout
         "mov %2, %%edx\n" // lengh of message
         "mov %3, %%rsi\n" // message to display
         "syscall\n"       // perform the syscall
         :
         :"m" (system_call_number),
          "m" (file_descriptor),
          "m" (message_length),
          "m" (message));
    return 0;
}
```
