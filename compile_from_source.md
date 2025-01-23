# Compile packages from  source code

Here is how to download source code of packages on an Ubuntu distro.
As of Ubuntu 24.10 repositories to get sources
are located in ```/etc/apt/sources.list.d/ubuntu.sources```.

The type of each repository is defined by a line:
```
Types: deb
```
In order to also download sources, simply add ```deb-src```:
```
Types: deb deb-src
```
That can be done automatically with the following command:
```
sudo sed -i 's/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu.sources
```
Then update the local immage of the content of repositories:
```
sudo apt update
```
For instance, now it's possible to dowload the source code of the bash package by just typing:
```
apt-get source bash
```

## Git
Here is how to download source code, perform compilation and install the binaries in directory ```/my/dir```. Type in a shell:
```bash
sudo apt install -y git gcc make automake gettext libcurl4-openssl-dev libssl-dev
apt source git
cd git-2.45.2
aclocal
autoconf
autoreconf --install --force --verbose
./configure --prefix=/my/dir --without-tcltk --with-curl=yes --with-openssl=yes --with-expat=yes 
make
sudo make install
```
To test the newly compiled git command:
```
/my/dir/bin/git clone https://github.com/canonical/cloud-init
```
