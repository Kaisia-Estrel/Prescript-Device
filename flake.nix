{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages."${system}";
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = [
          pkgs.arduino-language-server
          pkgs.arduino-cli
          pkgs.clang-tools
          pkgs.picocom
          pkgs.glibc.dev
        ];
      };

    };
}
