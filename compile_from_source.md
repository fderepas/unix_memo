# Compile some package from the sources

Here is how to download source code of packages on an Ubuntu distro.
As of Ubuntu 24.10 repositories to get sources
are located in ```/etc/apt/sources.list.d/ubuntu.sources```.

The type of each repository is defined by a line:
```
Types: deb
```
In order to also download sources, simply add ```deb-src```:
```
Types: deb-src
```
That can be done automatically with the following command
```
sudo sed -i '2/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu.sources
```
Then update the content of repositories:
```
sudo apt update
```
Then to dowload the source code of bash you can type:
```
apt-get source bash
```


## Git
To download, compile and install in ```/my/dir``` from source Git, type in a shell:
```bash
sudo apt install -y git gcc make automake gettext libcurl4-openssl-dev libssl-dev 
git clone https://github.com/git/git
cd git
aclocal
autoconf
autoreconf --install --force --verbose
./configure --prefix=/my/dir --without-tcltk --with-curl=yes --with-openssl=yes --with-expat=yes CFLAGS="${CFLAGS} -static"
make
sudo make install
```
To test the newly compiled git command:
```
/my/dir/bin/git clone https://github.com/canonical/cloud-init
```
