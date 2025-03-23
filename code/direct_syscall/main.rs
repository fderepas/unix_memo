use std::arch::asm;

fn main() {
    // variable representing the Linux syscall we want to call
    let syscall_number : i32 = 1;                    // 4 bytes
    // file descriptor on which to write
    let file_descriptor : i32 = 1;                   // 4 bytes
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
