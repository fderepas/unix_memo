// for more detail on gnu assembly:
// https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html

#include <stdio.h>

// compute the length of a string for strings smaller than 100 characters
int my_strlen(const char * s) {
    int answer=0;
    while(s[answer] && answer<100)
        ++answer;
    return answer;
}

int main() {
    // message to write (8 byte pointer)
    const char *message = "hello, world!\n";
    // length of the message to send (4 byte integer)
    int message_length = my_strlen(message);
    // file descriptor to write to (1=stdout)
    int file_descriptor=1;
    // number of the system call
    int system_call_number=1;
    __asm__ volatile
        ("mov %0, %%eax\n" // system call number (4 bytes)
         "mov %1, %%edi\n" // file descriptor: stdout (4 bytes)
         "mov %2, %%edx\n" // lengh of message (4 bytes)
         "mov %3, %%rsi\n" // message to display (8 bytes)
         "syscall\n"       // perform the syscall
         :
         :"m" (system_call_number),
          "m" (file_descriptor),
          "m" (message_length),
          "m" (message));
    return 0;
}
