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
