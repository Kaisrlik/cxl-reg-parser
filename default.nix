{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
	name = "cxl-reg-parser";
	src = ./.;

	# build and runtime depencies
	buildInputs = with pkgs; [ pciutils ];

	# build depencies
	nativeBuildInputs = with pkgs; [
		meson
		ninja
		pkg-config
	];
}
