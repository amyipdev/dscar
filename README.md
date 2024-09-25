# DSCar

This project is a work-in-progress for CPSC 440, developing a DSi-controllable car.

## Development

### Prerequisites

As long as you have Nix installed, you can build DSCar.
If you don't have Nix installed, check out [the Nix install page]("https://nixos.org/download").

Your Nix installation also needs to have the experimental features
`nix-command` and `flakes` enabled.

If you do not have a DSi or DSiXL (DSiXL recommended),
you may be able to do some basic testing in melonDS; however,
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
internally a `.nds` file, to your DSi's SD card.
