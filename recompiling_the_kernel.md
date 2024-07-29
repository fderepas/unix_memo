# Compile and run Linux Kernel 

##### Table of Contents  
[Compile new kernel source code](#compile-new-kernel-source-code)  
[Install the new kernel](#install-the-new-kernel)  
[Test the new kernel](#test-the-new-kernel)

## Compile new kernel source code

Get the sources of the kernel and compile them:

```bash
mkdir new_kernel
cd new_kernel/
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.9.9.tar.xz
tar xf linux-6.9.9.tar.xz
cd linux-6.9.9/
```


Get current system config:
```bash
cp -v /boot/config-$(uname -r) .config
```
Manual config (always answer yes):
```bash
make localmodconfig
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""
```
Do something <span name="patch">dumb</span> like patching the kernel to return values above 255 for a process (just an example to test a patch):

```bash
sed -i -e 's/\(error_code.*\)0xff/\10xfffffff/' kernel/exit.c
```
Or more seriously install [kernel-hardening-checker](https://github.com/a13xp0p0v/kernel-hardening-checker) (may affect system performance and functionality of userspace software) and type:
```bash
kernel-hardening-checker -g X86_64 > /tmp/fragment
./scripts/kconfig/merge_config.sh .config /tmp/fragment
```

Now compile!
```bash
fakeroot make -j`lscpu | grep '^CPU(s):' | tr -s ' ' | cut -d ' ' -f 2`
```
The command ```lscpu | grep '^CPU(s):' | tr -s ' ' | cut -d ' ' -f 2``` returns the numbers of cores on the current computer.


## Install the new kernel

We are going to install the kernel instance in a [multipass](https://multipass.run/) container. It works under Linux/MacOs/Windows.
Launch a new multipass instance:

```
multipass launch
```
It creates a new instance with a random name. If you want to force the name of the instance to ```foo``` you can type ```multipass launch --name foo```.
Let's assume the name of the lanched instance is ```wonderful-hairtail```. We can have the list of multipass instance with:

```
multipass list
```
Mount current directory under ```/home/ubuntu/kernel``` in the multipass instance:
```
multipass mount $PWD wonderful-hairtail:/home/ubuntu/kernel
```
Connect to the instance and then install the new kernel: 
```
multipass shell wonderful-hairtail
cd kernel
sudo apt install make
sudo make modules_install
sudo make install
sudo reboot
```
Now we are logged out of the multipass instance.

## Test the new kernel

We test the newly installed kernel with ```multipass shell wonderful-hairtail```. The command ```uname -a``` returns
```
Linux wonderful-hairtail 6.9.9 #1 SMP PREEMPT_DYNAMIC Sat Jul 13 09:19:26 CEST 2024 x86_64 x86_64 x86_64 GNU/Linux
```

Besides the kernel name, let's test the small patch we applied [previously](#patch)  . Here the following c code which returns a status:
```c
// display the return code of a forked process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
To remove the multipass image, type from the host machine:
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
```bash
sudo apt install gcc
gcc t.c && ./a.out
```
it gives the following output:
```
Child process (PID: 1796) terminated with 65536
```
To remove the multipass image, type from the host machine:
```bash
multipass delete -p wonderful-hairtail
```
