#!/bin/bash
# install zephyr workspace in /home/$USER/zephyrproject
set -x
set -e

cd
# install rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source ~/.cargo/env
cargo install --force cargo-make

# install opam
wget  https://opam.ocaml.org/install.sh
patch install.sh < ~/script/opam_install.patch 
bash install.sh
rm -rf install.sh
export OPAMCURL=curl
export OPAMFETCH=wget
echo | opam init --disable-sandboxing || echo
echo "test -r '/home/$USER/.opam/opam-init/init.sh' && . '/home/$USER/.opam/opam-init/init.sh' > /dev/null 2> /dev/null || true" >> ~/.profile
#opam update
#opam switch create 5.3.0+options
#opam switch set 5.3.0+options 
#echo | opam upgrade

### eval $(opam env --switch=default)

opam install --yes why3
opam install --yes why3-ide
opam install --yes why3find


# install creusot
mkdir -p ~/git
cd ~/git
git clone https://github.com/creusot-rs/creusot
cd creusot
DEFAULT_ANSWER=yes cargo run --bin creusot-install

