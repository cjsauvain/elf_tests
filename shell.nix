{
  pkgs ? import <nixpkgs> { },
}:

let
  # stdenv = pkgs.stdenvAdapters.useMoldLinker pkgs.llvmPackages_19.stdenv;
  stdenv = pkgs.llvmPackages_19.stdenv;
in
(pkgs.mkShell.override { inherit stdenv; }) {
  nativeBuildInputs = with pkgs; [
    nasm
    valgrind
    python312Packages.pyelftools
  ];
}

# vim: ts=2 sw=2 et 
