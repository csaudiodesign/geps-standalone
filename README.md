# GePS 2.0 - Gesture-based Performance System

_standalone gesture controlled musical instrument_

GePS reinvented. An evolved, integrated and standalone electronic musical instrument based on the research done on [GePS 1.0](https://github.com/csaudiodesign/GePS).

- Better form factor for easier wearability
- Lower latency for better performance experience through improved fusion of movement and sound
- Standalone sound module to be able to play the instrument without a laptop or workstation connected.

Furthermore we aim for better hackability through more programming interfaces ([sound module documentation](http://geps.synack.ch/doc-software.html)) and updated documentation as well as new and exciting sound modules ([geps-externals](https://github.com/csaudiodesign/geps-externals)!) and content.

This repository accompanies the developments that we document at our [Hackaday Project](https://hackaday.io/project/160886-geps).

## Dependencies

- [geps-externals](https://github.com/csaudiodesign/geps-externals)
- [comport](https://git.iem.at/pd/comport)
- [pd-cyclone](https://github.com/porres/pd-cyclone)
- [pd-freeverb](https://github.com/electrickery/pd-freeverb)
- [list-abs](https://github.com/pd-externals/list-abs)

## Installation

We suggest to build Pure data and the dependencies (externals) from sources to get latest and greatest versions (Pure data release 0.49 as the time of this writing), which are not available through the standard repositories.

In order to simplify the installation process of the Raspberry Pi sound module, we put together an Ansible playbook for provisioning. In addition to that, we integrated the build process into a script, which in turn is also run by the Ansible playbook.

```bash
cd setup
ansible-playbook --key-file ~/.ssh/rpi-provisioning \
    -u pi -i 192.168.88.181, geps-raspbian-playbook.yml
```

Here's a list of all the tasks that are automated by the playbook and buildscript:

- Upgrade raspbian
- Install the software dependencies for building Pure data and dependencies/externals (gcc, automake, ALSA and FFT dev libraries, libtool and git)
- Add the pisound package repository
- Install the pisound packages
- Disable unnecessary services (pisound-ctl, pisound-btn), as they consume a lot of resources at times
- Configure the default sound card (pisound)
- Running the build script, cloning, building and installing Pure data, dependencies and GePS sound module
  - Pure data (vanilla, release 0.49)
  - pd-cyclone
  - list-abs (just install, no building required)
  - pd-freeverb
  - comport
  - geps-externals (our GePS externals library)
  - geps-standalone (this repository)
- Enable starting the GePS sound module (with a systemd system unit) when booting the Raspberry Pi
- Building Pure data and dependencies from sources, install the GePS package
