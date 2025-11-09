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
Here is an example of script to download:
```bash
# setup.sh
set -e
set -x
HOSTNAME=my-host-name
USERNAME=ubuntu
apt update
apt upgrade -y 
apt install -y network-manager figlet byobu btop git
figlet $HOSTNAME > /etc/motd
cat << EOF >> /etc/cloud/cloud.cfg.d/01-hostname.cfg
preserve_hostname: true           # prevent cloud-init from setting the hostname
manage_etc_hosts: true            # have cloud-init keep /etc/hosts in sync
prefer_fqdn_over_hostname: false  # do not use FQDN for the system hostname
EOF
hostnamectl set-hostname $HOSTNAME
# download and execute the user init script
bash -lc 'set -euo pipefail; curl -fsSL http://example.com/init.sh -o /home/$USERNAME/init.sh'
echo /home/$USERNAME/init.sh | su - $USERNAME
```
