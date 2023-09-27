{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
	name = "cxl-reg-parser";
	src = ./.;

	# build and runtime depencies
	buildInputs = with pkgs; [ ];

	# build depencies
	nativeBuildInputs = with pkgs; [ ];
}
