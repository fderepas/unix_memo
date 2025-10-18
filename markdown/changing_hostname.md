There are several ways to set the hostname in Ubuntu. Here are two frequent methods.
# Cloud-init

Create a file named `/etc/cloud/cloud.cfg.d/01-hostname.cfg` which contains:
```yaml
preserve_hostname: false          # let cloud-init set the hostname
hostname: myhostname              # short hostname
fqdn: myhostname.example.com      # optional but recommended
manage_etc_hosts: true            # have cloud-init keep /etc/hosts in sync
prefer_fqdn_over_hostname: false  # do not use FQDN for the system hostname
```

To disable cloud init:
```yaml
preserve_hostname: true        # don’t reset hostname
manage_etc_hosts: false        # don’t rewrite /etc/hosts
network: {config: disabled}    # let Netplan/you manage networking
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

# Manually

```
sudo hostnamectl set-hostname myhostname
```
