BUILDDIR := build/

all: $(BUILDDIR)/.build compile

$(BUILDDIR)/.build: $(BUILDDIR)
	touch $@

$(BUILDDIR):
	meson setup -Db_sanitize=address $@

compile:
	meson compile -C $(BUILDDIR)

test:
	meson test -C $(BUILDDIR)

nix:
	nix develop --build

clean:
	rm -rf $(BUILDDIR)
