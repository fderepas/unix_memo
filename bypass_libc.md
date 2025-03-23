This page presents how to bypass the c-library.

The 64-bit system call numbers and entry vectors is given by [syscall_64.tbl](https://github.com/torvalds/linux/blob/v6.7/arch/x86/entry/syscalls/syscall_64.tbl).

It can be found in the lib-c or in [Rust Syscall](https://docs.rs/syscalls/0.6.18/src/syscalls/arch/x86_64.rs.html) crate.

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
Code is [here](https://github.com/fderepas/unix_memo/blob/main/code/bypass/main.c).

Here is the equivalent in Rust:
```rust
use std::arch::asm;

fn main() {
    let syscall_number : i32 = 1;                // 4 bytes
    let file_descriptor : u64 = 1;               // 4 bytes
    let message = b"Hello, world!\n";
    let message_length : usize = message.len();  // 8 bytes
    let message_ptr : * const u8 = message.as_ptr(); // 8 bytes
    unsafe {
        asm!(
            "mov eax, {0:e}",
            "mov edi, {1:e}",
            "mov rdx, {2:r}",
            "mov rsi, {3:r}",
            "syscall",
            in(reg) syscall_number,
            in(reg) file_descriptor,
            in(reg) message_length,
            in(reg) message_ptr
        );
    }
}
```
Code is [here](https://github.com/fderepas/unix_memo/blob/main/code/bypass/main.rs).