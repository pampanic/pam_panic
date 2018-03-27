# pam\_panic


## Purpose
The pam\_panic PAM module shall protect people who have value data on their computer. It provides a panic function.


## Installation
You need gcc or something similar.
To compile and install it you may want to do the following within this project directory:

```
make
sudo make install
```

### Compiling notes
The Makefile passes the pathes of `reboot`, `poweroff` and `cryptsetup` using macros to be sure that it will run on different machines.
You need libpam's development package. Some call them like `libpam0g-dev`.


## Preparation
You need two GPT formatted removable devices. There must be at least one partition on it. Here is an example with `fdisk`:

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

You'll find the UUID of your partition in `/dev/disk/by-partuuid/`. You can find out which device is which through a `ls -l /dev/disk/by-partuuid/`.


## Integration
To let it integrate with your system, add the following at the top of of your pam.d config(s):


```
auth       requisite    pam_panic.so auth=<UUID> reject=<UUID> reboot serious=<UUID>
account    requisite    pam_panic.so
```

See `man 8 pam_panic` for more.


## TODO
Asking for man page translations.


