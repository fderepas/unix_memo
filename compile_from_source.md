# Compile some package from the sources

## Git
To download, compile and install in ```/my/dir``` from source Git, type in a shell:
```bash
sudo apt install -y git gcc make automake gettext libcurl4-openssl-dev libssl-dev 
git clone https://github.com/git/git
cd git
aclocal
autoconf
automake --add-missing
autoreconf --install --force --verbose
./configure --prefix=/my/dir --without-tcltk --with-curl=yes --with-openssl=yes --with-expat=yes
make
sudo make install
```
To test the newly compiled git command:
```
/my/dir/bin/git clone https://github.com/canonical/cloud-init
```
