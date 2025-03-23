This page presents how to perform direct system calls from user space to the kernel in C and in Rust.

# Linux on x86_64

The 64-bit system call numbers and entry vectors in Linux is given by [syscall_64.tbl](https://github.com/torvalds/linux/blob/v6.7/arch/x86/entry/syscalls/syscall_64.tbl).

It can be found in the lib-c or in [Rust Syscall](https://docs.rs/syscalls/0.6.18/src/syscalls/arch/x86_64.rs.html) crate.

## In C

Here is an 'hello world' program in C with direct Linux system calls in assembly:
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
Code is [here](https://github.com/fderepas/unix_memo/blob/main/code/direct_syscall/main.c).

## Rust

Here is the equivalent in Rust:
```rust
use std::arch::asm;

fn main() {
    // variable representing the Linux syscall we want to call
    let syscall_number : i32 = 1;                    // 4 bytes
    // file descriptor on which to write
    let file_descriptor : u64 = 1;                   // 4 bytes
    // buffer to write
    let message = b"Hello, world!\n";
    // number of bytes to write
    let message_length : usize = message.len();      // 8 bytes
    // pointer to the buffer
    let message_ptr : * const u8 = message.as_ptr(); // 8 bytes
    unsafe {
        asm!(
            "mov eax, {0:e}", // move argument #0 (syscall_number) in eax
            "mov edi, {1:e}", // move argument #1 (file_descriptor) in edi
            "mov rdx, {2:r}", // move argument #2 (message_length) in rdx
            "mov rsi, {3:r}", // move argument #3 (message_ptr) in rsi
            "syscall",
            in(reg) syscall_number,
            in(reg) file_descriptor,
            in(reg) message_length,
            in(reg) message_ptr
        );
    }
}
```
Code is [here](https://github.com/fderepas/unix_memo/blob/main/code/direct_syscall/main.rs).

