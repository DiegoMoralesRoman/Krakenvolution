{
  description = "A simple C++ project with SFML and gtest";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            cmake
            sfml
            gtest
          ];

          # Set any environment variables or shell hooks you need for development
        };

        # You can also define a `packages` attribute if you want to build and package your application
      });
}
