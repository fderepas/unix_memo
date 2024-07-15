## Kernel compilation

Get the sources of the kernel and compile them:

```
mkdir new_kernel
cd new_kernel/
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.9.9.tar.xz
tar xf linux-6.9.9.tar.xz
cd linux-6.9.9/
```
Do something dumb like partching the kernel to return values above 255 for a process:

```
sed -i -e 's/\(error_code.*\)0xff/\10xfffffff/' kernel/exit.c
```

Get current system config:
```
cp -v /boot/config-$(uname -r) .config
```
Manual config (always answer yes):
```
make localmodconfig
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""
fakeroot make -j`lscpu | grep '^CPU(s):' | tr -s ' ' | cut -d ' ' -f 2`
```
The command ```lscpu | grep '^CPU(s):' | tr -s ' ' | cut -d ' ' -f 2``` returns the numbers of cores on the current computer.

## Install the new kernel

We are going to install the kernel instance in a [multipass](https://multipass.run/) container in order to ensure the stability of the system.
Launch a new multipass instance:

```
multipass launch
```
It creates a new instance with a random name.
Let's assume the name of the lanched instance is ```wonderful-hairtail```. We can have the list of multipass instance with:

```
multipass list
```
Mount currenbt directory under ```/home/ubuntu/kernel``` in the multipass instance, log in the instance and install the new kernel: 
```
multipass mount $PWD wonderful-hairtail:/home/ubuntu/kernel
multipass shell wonderful-hairtail
cd kernel
sudo apt install make
sudo make modules_install
sudo make install
sudo reboot
```
Now we are logged out of the multipass instance. We can reconnect with ```multipass shell wonderful-hairtail```. The command ```uname -a``` returns
```
Linux wonderful-hairtail 6.9.9 #1 SMP PREEMPT_DYNAMIC Sat Jul 13 09:19:26 CEST 2024 x86_64 x86_64 x86_64 GNU/Linux
```

## Testing the new kernel

Here the following c code:
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main() {
    pid_t pid;
    int status;
 
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        return(1<<8);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process (PID: %d) terminated with status %d\n",
                   pid, status);
        } else {
            printf("Child process (PID: %d) did not terminate normally\n", pid);
        }
    }
    return 0;
}
```
Let's compile and execute:
```
sudo apt install gcc
gcc t.c && ./a.out
```
it gives the following output:
```
Child process (PID: 1796) terminated with 65536
```
