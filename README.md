[![Build Status](https://travis-ci.org/pampanic/pam_panic.svg?branch=master)](https://travis-ci.org/pampanic/pam_panic)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/001445f05a4e47bcad151c2f6a05a29c)](https://app.codacy.com/app/pampanic/pam_panic?utm_source=github.com&utm_medium=referral&utm_content=pampanic/pam_panic&utm_campaign=badger)
# pam\_panic


## Purpose
pam\_panic is a PAM module that protects sensitive data and provides a panic function for emergency situations.


## How it works
You can choose from one of two options:

### Using two removable media previous your own password
There are two removable media which work as keys: the auth key and the panic key. The auth key will let you pass to the password prompt whereas the panic key, if provided, will securely erase the LUKS header, rendering the data unreadable.

### Using two passwords previous your own password
There are two passwords you are able to set: the key password and the panic password. The key password will let you pass to the original password prompt whereas the panic password, if provided, will securely erase the LUKS header, rendering the data unreadable.


## Installation
### Arch Linux AUR
There is
- [pam_panic](https://aur.archlinux.org/packages/pam_panic/) - Actual [releases](https://github.com/pampanic/pam_panic/releases) and
- [pam_panic-git](https://aur.archlinux.org/packages/pam_panic-git/) for this actual git repository.


### Ubuntu
There's a [PPA](https://launchpad.net/~bandie/+archive/ubuntu/pampanic) updating for new [releases](https://github.com/pampanic/pam_panic/releases).

To install the package using the PPA:
```
sudo add-apt-repository ppa:bandie/pampanic
sudo apt-get update
sudo apt-get install pampanic
```

### From sources
You will need GCC or similar, as well as the PAM headers. Some distributions package the PAM headers as `libpam0g-dev`.
Also you need `autoconf`.

To compile and install it, do the following within the project's root directory:

```console
$ [ ! -e ./configure ] && autoreconf -i
$ ./configure
$ make
$ sudo make install
```

Note: the paths of the `reboot`, `poweroff`, and `cryptsetup` commands are passed to the module at compile-time.

## Preparation

If you want to use removable media you'll need two GPT-formatted removable storage devices, and said devices must have at least one partition. Here's an example `fdisk` session, showing how this might be accomplished:

```console
$ sudo fdisk /dev/sdc

Welcome to fdisk (util-linux 2.31.1).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.


Command (m for help): g
Created a new GPT disklabel (GUID: AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA).

Command (m for help): n
Partition number (1-128, default 1): 
First sector (2048-15661022, default 2048): 
Last sector, +sectors or +size{K,M,G,T,P} (2048-15661022, default 15661022): 

Created a new partition 1 of type 'Linux filesystem' and of size 7.5 GiB.
Command (m for help): w
```

You'll find the UUID of your partition in `/dev/disk/by-partuuid/`. You can find out which device is which typing `ls -l /dev/disk/by-partuuid/` in your favourite shell.



## Configuration
To configure the module, add the following to the appropriate PAM configuration file(s): (see `pam.conf(5)` for details on these files)

### Using the removable media:
```
auth       requisite    /usr/local/lib/security/pam_panic.so auth=<UUID> reject=<UUID> reboot serious=<UUID>
account    requisite    /usr/local/lib/security/pam_panic.so
```

### Using the two passwords:
```
auth       requisite    /usr/local/lib/security/pam_panic.so password reboot serious=<UUID>
account    requisite    /usr/local/lib/security/pam_panic.so
```
To set your passwords run `pam_panic_pw` as root in your preferred shell.


## More information
See `man 8 pam_panic` and `man 1 pam_panic_pw` for more information.


## Contact
- Support channel on IRC at [**#pampanic** on chat.**freenode**.net](https://webchat.freenode.net/?channels=%23pampanic&uio=d4)  - Feel free to idle. :)
- Developer channel on IRC at [**#pampanic-dev** on chat.**freenode**.net](https://webchat.freenode.net/?channels=%23pampanic-dev&uio=d4) 


## TODO
- [Manpage translations](https://github.com/Bandie/pam_panic/issues?q=is%3Aissue+is%3Aopen+label%3Alocalization)


# Addendum

## Poisoning memory when issuing a reboot or shutdown

If you want to be sure to have your memory clear of all information when issuing a reboot/shutdown 
you might want to add the option `page_poison=on` and `slub_debug=P` to your kernel command line at boot.
For GRUB2 you just append it on your `GRUB_CMDLINE_LINUX` entry in `/etc/default/grub` and then issue a rebuild
of the GRUB2 config: `grub-mkconfig -o /boot/grub/grub.cfg`


