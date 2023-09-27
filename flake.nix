{
	description = "cxl-reg-parser";
	inputs.nixpkgs.url = "nixpkgs/nixos-23.05";

	outputs = {self, nixpkgs}:
	let
		system = "x86_64-linux";
		pkgs = nixpkgs.legacyPackages.${system};
	in
	{
		packages.${system}.default = (import ./default.nix { inherit pkgs; });
	};
}
