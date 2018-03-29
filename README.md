# pam\_panic

[![Build Status](https://travis-ci.org/Bandie/pam_panic.svg?branch=master)](https://travis-ci.org/Bandie/pam_panic)


## Purpose
pam\_panic is a PAM module that protects sensitive data and provices a panic function for emergency situations.

## How it works
There are two removable media which work as keys: the auth key and the panic key. The auth key will let you pass to the password prompt whereas the panic key, if provided, will securely erase the LUKS header, rendering the data unreadable.

## Installation
You will need GCC or similar, as well as the PAM headers. Some distributions package the PAM headers as `libpam0g-dev`.

To compile and install it, do the following within the project's root directory:

```
make
sudo make install
```

Note: the paths of the `reboot`, `poweroff`, and `cryptsetup` commands are passed to the module at compile-time.

## Preparation
You'll need two GPT-formatted removable storage devices, and said devices must have at least one partition. Here's an example `fdisk` session, showing how this might be accomplished:

```
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
To configure the module, add the following to the appropriate PAM configuration file(s): (see pam.conf(5) for details on these files)


```
auth       requisite    pam_panic.so auth=<UUID> reject=<UUID> reboot serious=<UUID>
account    requisite    pam_panic.so
```

See `man 8 pam_panic` for more.


## TODO
- [Manpage translations](https://github.com/Bandie/pam_panic/issues?q=is%3Aissue+is%3Aopen+label%3Alocalization)
- Integrate [panic password](https://github.com/Bandie/pam_panic/issues/7)
