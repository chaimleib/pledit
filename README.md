# pledit
Plist tools

Requires:
* libplist - https://github.com/libimobiledevice/libplist
* libyaml - https://bitbucket.org/xi/libyaml

## Installing
```bash
./configure
make
make install
```

## Autotools - Devs only
To regenerate the autotools:
```bash
aclocal
automake --add-missing
autoconf
```
