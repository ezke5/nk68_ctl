# nk68_ctl

`nk68_ctl` is a small C11 command-line controller for the Ajazz / NACODEX NK68
keyboard backlight. It uses the Linux `hidraw` feature reports for USB device
`258a:013b`, interface `:1.1`.

## Build

Linux is the supported runtime platform. Portable builds on other systems
compile the command-line layer and report that the HID backend is unsupported.

```sh
make
```

The build uses `-O2`, dependency files, warnings, and separate modules for CLI,
device discovery, HID transport, and keyboard protocol handling.

## Install

The default installation is `/usr/local/bin/nk68_ctl`:

```sh
sudo make install
sudo udevadm control --reload-rules
sudo udevadm trigger
```

This also installs the man page, Bash completion, and
`/etc/udev/rules.d/99-nk68.rules`. The udev rule grants the `plugdev` group
read/write access and adds `uaccess` for a locally active desktop session.
Add your user to `plugdev` if your distribution uses that group:

```sh
sudo usermod -aG plugdev "$USER"
```
`If plugdev group does not exist; simply create, then execute the command.`

Afterward, log out and back in after changing group membership. Without the udev rule,
the program may require access to `/dev/hidrawN`; do not use broad `chmod 666`
rules.

## Usage

```text
nk68_ctl --get
nk68_ctl --mode 1 --speed 3 --brightness 5
nk68_ctl 1 3 5
nk68_ctl 13
```

Modes are `1-13`, speeds are `1-5`, and brightness is `0-5`. Mode 13 turns the
backlight off.

## GitHub Actions

The workflow in `.github/workflows/build.yml` performs native builds and smoke
test on Ubuntu, then uploads a platform artifact.

## Attention
**Only Linux can communicate with the keyboard.**

## License

MIT. See [LICENSE](LICENSE).
