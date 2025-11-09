Here is a way to customize a Raspberry Pi using cloud-init.

# Creating the initial image
On a PC install `rpi-imager`:
```
sudo apt install rpi-imager
```
Customize the image to connect to a network and create a default user. After flashing, re-insert the card. That mounts `system-boot` under `/media/$USER/`. In this directory open the file `user-data` and add:
```yaml
runcmd:
  - bash -lc 'set -euo pipefail; curl -fsSL http://example.com/setup.sh -o /usr/local/bin/setup.sh'
  - chmod +x /usr/local/bin/setup.sh
  - /usr/local/bin/setup.sh
```
