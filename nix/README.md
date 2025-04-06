# Shattered Prism Nix Packaging

## Installing a development release (flake)

We use [cachix](https://cachix.org/) to cache our development and release builds.
If you want to avoid rebuilds you may add the Cachix bucket to your substitutors, or use `--accept-flake-config`
to temporarily enable it when using `nix` commands.

Example (NixOS):

```nix
{
  nix.settings = {
    trusted-substituters = [ "https://unmojang.cachix.org" ];

    trusted-public-keys = [
      "unmojang.cachix.org-1:OfHnbBNduZ6Smx9oNbLFbYyvOWSoxb2uPcnXPj4EDQY="
    ];
  };
}
```

### Installing the package directly

After adding `github:lunaislazier/ShatteredPrism` to your flake inputs, you can access the flake's `packages` output.

Example:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    shatteredprism = {
      url = "github:lunaislazier/ShatteredPrism";

      # Optional: Override the nixpkgs input of shatteredprism to use the same revision as the rest of your flake
      # Note that this may break the reproducibility mentioned above, and you might not be able to access the binary cache
      #
      # inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    { nixpkgs, shatteredprism, ... }:
    {
      nixosConfigurations.foo = nixpkgs.lib.nixosSystem {
        modules = [
          ./configuration.nix

          (
            { pkgs, ... }:
            {
              environment.systemPackages = [ shatteredprism.packages.${pkgs.system}.shatteredprism ];
            }
          )
        ];
      };
    };
}
```

### Using the overlay

Alternatively, if you don't want to use our `packages` output, you can add our overlay to your nixpkgs instance.
This will ensure Shattered Prism is built with your system's packages.

> [!WARNING]
> Depending on what revision of nixpkgs your system uses, this may result in binaries that differ from the above `packages` output
> If this is the case, you will not be able to use the binary cache

Example:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    shatteredprism = {
      url = "github:lunaislazier/ShatteredPrism";

      # Optional: Override the nixpkgs input of shatteredprism to use the same revision as the rest of your flake
      # Note that this may break the reproducibility mentioned above, and you might not be able to access the binary cache
      #
      # inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    { nixpkgs, shatteredprism, ... }:
    {
      nixosConfigurations.foo = nixpkgs.lib.nixosSystem {
        modules = [
          ./configuration.nix

          (
            { pkgs, ... }:
            {
              nixpkgs.overlays = [ shatteredprism.overlays.default ];

              environment.systemPackages = [ pkgs.shatteredprism ];
            }
          )
        ];
      };
    };
}
```

### Installing the package ad-hoc (`nix shell`, `nix run`, etc.)

You can simply call the default package of this flake.

Example:

```shell
nix run github:lunaislazier/ShatteredPrism

nix shell github:lunaislazier/ShatteredPrism

nix profile install github:lunaislazier/ShatteredPrism
```

## Installing a development release (without flakes)

We use [Cachix](https://cachix.org/) to cache our development and release builds.
If you want to avoid rebuilds you may add the Cachix bucket to your substitutors.

Example (NixOS):

```nix
{
  nix.settings = {
    trusted-substituters = [ "https://unmojang.cachix.org" ];

    trusted-public-keys = [
      "unmojang.cachix.org-1:OfHnbBNduZ6Smx9oNbLFbYyvOWSoxb2uPcnXPj4EDQY="
    ];
  };
}
```

### Installing the package directly (`fetchTarball`)

We use flake-compat to allow using this Flake on a system that doesn't use flakes.

Example:

```nix
{ pkgs, ... }:
{
  environment.systemPackages = [
    (import (
      builtins.fetchTarball "https://github.com/ShatteredPrism/ShatteredPrism/archive/develop.tar.gz"
    )).packages.${pkgs.system}.shatteredprism
  ];
}
```

### Using the overlay (`fetchTarball`)

Alternatively, if you don't want to use our `packages` output, you can add our overlay to your instance of nixpkgs.
This results in Shattered Prism using your system's libraries

Example:

```nix
{ pkgs, ... }:
{
  nixpkgs.overlays = [
    (import (
      builtins.fetchTarball "https://github.com/ShatteredPrism/ShatteredPrism/archive/develop.tar.gz"
    )).overlays.default
  ];

  environment.systemPackages = [ pkgs.shatteredprism ];
}
```

### Installing the package ad-hoc (`nix-env`)

You can add this repository as a channel and install its packages that way.

Example:

```shell
nix-channel --add https://github.com/lunaislazier/ShatteredPrism/archive/develop.tar.gz shatteredprism

nix-channel --update shatteredprism

nix-env -iA shatteredprism.shatteredprism
```

## Package variants

This repository offers the following packages:

- `shatteredprism` - The preferred build, wrapped with everything necessary to run the launcher and Minecraft
- `shatteredprism-unwrapped` - A minimal build that allows for advanced customization of the launcher's runtime environment

### Customizing wrapped packages

The wrapped package (`shatteredprism`) offers some build parameters to further customize the launcher's environment.

The following parameters can be overridden:

- `additionalLibs` (default: `[ ]`) Additional libraries that will be added to `LD_LIBRARY_PATH`
- `additionalPrograms` (default: `[ ]`) Additional libraries that will be added to `PATH`
- `controllerSupport` (default: `isLinux`) Turn on/off support for controllers on Linux (macOS will always have this)
- `gamemodeSupport` (default: `isLinux`) Turn on/off support for [Feral GameMode](https://github.com/FeralInteractive/gamemode) on Linux
- `jdks` (default: `[ jdk21 jdk17 jdk8 ]`) Java runtimes added to `SHATTEREDPRISM_JAVA_PATHS` variable
- `msaClientID` (default: `null`, requires full rebuild!) Client ID used for Microsoft Authentication
- `textToSpeechSupport` (default: `isLinux`) Turn on/off support for text-to-speech on Linux (macOS will always have this)
- `withWaylandGLFW` (default: `isLinux`) Build with support for native Wayland via a custom GLFW
