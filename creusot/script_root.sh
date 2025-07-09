#!/bin/bash
# script to be executed as root to prepare the system to install creusot

apt update 
apt upgrade -q -y 
apt install -y unzip bubblewrap make gcc git wget libssl-dev openssl libssl3t64 pkg-config sudo bzip2 curl autoconf libcairo2-dev libexpat1-dev libgmp-dev libgtk-3-dev libgtksourceview-3.0-dev libzmq3-dev zlib1g-dev  apparmor-profiles rsync adduser

apt install -y waypipe

ln -s /usr/share/apparmor/extra-profiles/bwrap-userns-restrict /etc/apparmor.d/
systemctl reload apparmor

# use sudo without password
mkdir -p /etc/sudoers.d
echo "ubuntu ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/ubuntu

