
crontab to update ubuntu each night:
```bash
$ sudo crontab -l
0 1 * * * apt update
0 2 * * * apt upgrade -y
0 3 * * * /bin/bash /root/reboot_if_needed.sh
```
Where ```/root/reboot_if_needed.sh``` is a script which reboots randomly if needed in the upcoming hour:

```bash
#!/bin/bash
sleep $((RANDOM % 3600))
[ -e /run/reboot-required ] && shutdown -r now
```
Reboot time is random so that not all machines reboot simultaneously.
