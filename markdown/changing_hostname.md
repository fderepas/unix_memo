# TL;DR
With Ubuntu and cloud init activated (default config): create a file named `/etc/cloud/cloud.cfg.d/01-hostname.cfg` which contains:
```yaml
preserve_hostname: false          # present cloud-init from setting the hostname
manage_etc_hosts: true            # have cloud-init keep /etc/hosts in sync
prefer_fqdn_over_hostname: false  # do not use FQDN for the system hostname
```
Then manually set the name:

```
sudo hostnamectl set-hostname myhostname
```

# Cloud-init

To use cloud-init to set the host name:
```yaml
preserve_hostname: true           # let cloud-init set the hostname
hostname: myhostname              # short hostname
fqdn: myhostname.example.com      # optional but recommended
manage_etc_hosts: true            # have cloud-init keep /etc/hosts in sync
prefer_fqdn_over_hostname: false  # do not use FQDN for the system hostname
```

To turn off cloud init:
- disable: create an empty file `/etc/cloud/cloud-init.disabled` and reboot.
- remove: `sudo apt purge cloud-init` (make sure any network config cloud-init created has been replaced).

# DHCP

To disable DHCP via netplan:

```yaml
# /etc/netplan/50-disable-dhcp-hostname.yaml
network:
  version: 2
  ethernets:
    eth0:
      dhcp4: true
      dhcp4-overrides:
        use-hostname: false
      # dhcp6-overrides:
      #   use-hostname: false
```
Apply it:

```
sudo netplan apply
```
