# DSCar

This project is a work-in-progress for CPSC 440, developing a DSi-controllable car.

## Development

### Prerequisites

As long as you have Nix installed, you can build DSCar.
If you don't have Nix installed, check out [the Nix install page]("https://nixos.org/download").

Your Nix installation also needs to have the experimental features
`nix-command` and `flakes` enabled.

If you do not have a DSi or DSiXL (DSiXL recommended),
you may be able to do some basic software testing in melonDS; however,
this should be considered an **unsupported** method.

### DSi App

Building the app is simple, just run:

``` sh
nix build
```

Check out `dsi/Makefile` for build information and to add new libraries,
source directories, graphics, and more. If you modify the Makefile or add
new files, you need to run the following command:

``` sh
nix develop --command make bear
```

That command also needs to be run on your first build if you are using
a clangd-based LSP in your IDE.

If you are running on real hardware, you should then copy `result`, which is
internally a `.nds` file, to your DSi's SD card as `dscar.nds`.

### Raspberry Pi - Software

#### Raspberry Pi Zero W/Zero WH

*Note: this may also work on the Zero 2 W/WH, 3B, 3B+, 3A+, 4/400, and 5; they are not recommended as they have higher power draw*

##### Part 1

These instructions work without a keyboard/display, but require a wireless network.
You need one or the other; the Pi can serve as a network for the DSi post-installation.
Your network also needs to show the Pi's IP address.

TODO: pictures, verify step names

First, download `rpi-imager` from your distribution repository. 
Load up your SD card and `rpi-imager`. Select your Pi model, 
Other Raspberry Pi OS > Lite, and then your SD card.

After proceeding, enter OS configuration. Set your
timezone, hostname (such as DSCar), and wifi info (if applicable).
You also need to enable SSH in Services.

Flash the SD card, and then insert it into the Pi and power it.
If you are working wirelessly, it may take a few minutes for the Pi to connect
to your network; you need to wait for this to happen. Keyboard users should wait for
the login prompt.

You now need to log into the Pi. For wireless users, connect another device to the
same network as the Pi, and then run `ssh username@ipaddress`; this will allow you 
to log in. 

If you are going to use the Pi's hotspot functionality, continue to Part 2.
If you're using a static network, configure it with `nmcli` or `nmtui`, and then
continue to Part 3. Hotspot functionality is recommended unless you need to control 
the car over a long-distance network. Note that your network must be open (no password)
or WEP; WPA/WPA2/WPA3 is not supported by the DSi.

##### Part 2

Wireless users should now `nmcli con modify preconfigured autoconnect no`
once you are sure your connection is stable.

Next, escalate your privileges and generate the hotspot:

``` sh
sudo su
nmcli con add type wifi ifname wlan0 con-name Hotspot autoconnect yes ssid DSCar-YOURCARNAME
nmcli con modify Hotspot 802-11-wireless.mode ap 802-11-wireless.band bg ipv4.method shared
# Optionally, set up WEP if you wish
nmcli con up Hotspot
```

Wireless users will now get disconnected from SSH. You need to connect to the DSCar network,
and change the IP address to the IP address of your gateway (should be `10.42.0.1`).

##### Part 3

TODO: Installing the car control software

#### Raspberry Pi Pico W/Pico WH

*Note: the Pico 2 is not supported, as it does not have wireless support.*

TODO
