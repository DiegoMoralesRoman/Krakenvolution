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
			protobuf
			yaml-cpp
          ];

		 shellHook = ''
            export SHELL=${pkgs.zsh}/bin/zsh
          '';         # Set any environment variables or shell hooks you need for development
        };

		# TODO: make proper build step
        packages = {
          my-app = pkgs.stdenv.mkDerivation {
            name = "my-app";
            src = self;
            buildInputs = with pkgs; [ gcc cmake sfml gtest ];
            # Replace this with your actual build commands
            buildPhase = ''
              cmake .
              make
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp my-app $out/bin/
            '';
          };
        };

        defaultPackage = self.packages.my-app;
      });
}
