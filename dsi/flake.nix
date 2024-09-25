# SPDX-License-Identifier: GPL-2.0-or-later
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    devkitNix.url = "github:bandithedoge/devkitNix";
  };
  outputs = {self, nixpkgs, flake-utils, devkitNix}:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
        overlays = [devkitNix.overlays.default];
      };
    in {
      devShells.default = pkgs.mkShell {
        buildInputs = [pkgs.devkitNix.devkitARM pkgs.bear];
        inherit (pkgs.devkitNix.devkitARM) shellHook;
      };
      packages.default = pkgs.stdenv.mkDerivation rec {
        name = "dscar-dsi";
        src = ./.;
        makeFlags = ["TARGET=dscar"];
        preBuild = pkgs.devkitNix.devkitARM.shellHook;
        installPhase = ''
          cp dscar.nds $out
        '';
      };
    });
}
