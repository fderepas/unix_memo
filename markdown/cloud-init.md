Here is a way to customize a Raspberry Pi using cloud-init.

# Creating the initial image
On a PC install `rpi-imager`:
```
sudo apt install rpi-imager
```
Customize the image to connect to a network and create a default user for instance `myuser`. After flashing, re-insert the card. That mounts `system-boot` under `/media/$USER/`. In this directory open the file `user-data` and add:
```yaml
runcmd:
  - [bash, '-lc', 'curl -fsSL http://example.com/setup.sh -o /usr/local/bin/setup.sh' ]
  - [chmod, '+x', '/usr/local/bin/setup.sh' ]
  - [/usr/local/bin/setup.sh, '--hostname', 'my-host-name', '--username myuser', '--server-url', 'http://example.com/init.sh' ]
```

# Init script
Here is an example of script to download:
```bash
#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<EOF
Usage: $(basename "$0") --hostname HOSTNAME --username jane --server-url https://example.com/init.sh

EOF
}

HOST=""
USER_NAME=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --hostname)
      [[ $# -ge 2 ]] || { echo "Error: --hostname requires an argument." >&2; usage; exit 2; }
      [[ -z "$HOST" ]] || { echo "Error: --hostname specified more than once." >&2; exit 2; }
      [[ "$2" != -* ]] || { echo "Error: --hostname value looks like an option: '$2'." >&2; exit 2; }
      HOST="$2"
      shift 2
      ;;
    --username)
      [[ $# -ge 2 ]] || { echo "Error: --username requires an argument." >&2; usage; exit 2; }
      [[ -z "$USER_NAME" ]] || { echo "Error: --username specified more than once." >&2; exit 2; }
      [[ "$2" != -* ]] || { echo "Error: --username value looks like an option: '$2'." >&2; exit 2; }
      USER_NAME="$2"
      shift 2
      ;;
    --server-url)
      [[ $# -ge 2 ]] || { echo "Error: --server-url requires an argument." >&2; usage; exit 2; }
      [[ -z "$SERVER_URL" ]] || { echo "Error: --server-url specified more than once." >&2; exit 2; }
      [[ "$2" != -* ]] || { echo "Error: --server-url value looks like an option: '$2'." >&2; exit 2; }
      SERVER_URL="$2"
      shift 2
      ;;
    -h|--help)
      usage; exit 0
      ;;
    --)
      shift; break
      ;;
    -*)
      echo "Error: Unknown option: $1" >&2; usage; exit 2
      ;;
    *)
      echo "Error: Unexpected positional argument: $1" >&2; usage; exit 2
      ;;
  esac
done

if [[ -z "$HOST" || -z "$USER_NAME" || -z "$SERVER_URL" ]]; then
  echo "Error: --hostname --server-url and --username are required." >&2
  usage
  exit 2
fi

apt update
apt upgrade -y 
apt install -y network-manager figlet byobu btop git
figlet $HOST > /etc/motd
cat << EOF >> /etc/cloud/cloud.cfg.d/01-hostname.cfg
preserve_hostname: true           # prevent cloud-init from setting the hostname
manage_etc_hosts: true            # have cloud-init keep /etc/hosts in sync
prefer_fqdn_over_hostname: false  # do not use FQDN for the system hostname
EOF
hostnamectl set-hostname $HOST
# download and execute the user init script
bash -lc 'set -euo pipefail; curl -fsSL $SERVER_URL -o /home/$USER_NAME/init.sh'
chmodm +x /home/$USER_NAME/init.sh
echo /home/$USER_NAME/init.sh | su - $USERNAME
```
