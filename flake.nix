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
			yaml-cpp
			platformio
			protobuf
          ];

		 shellHook = ''
			alias run="make -j4 && ./src/krakenvolution"
			alias sender="make -j4 && ./src/msgsender"
            export SHELL=${pkgs.zsh}/bin/zsh
			export CPATH=$(lib.makeSearchPathOutput "dev" "include" buildInputs)

			export LC_CTYPE=C.utf8
			export LC_ALL=C.utf8
          '';         # Set any environment variables or shell hooks you need for development
        };

      });
}
