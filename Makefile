# Specify phony targets to avoid conflicts with files named 'all' or 'install'
.PHONY: all install format build

# Default target: builds the project
all:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make

# Install target: installs the compiled binary
install:
	sudo make -C build install
	sudo cp build/newlinux /usr/bin/newlinux

# Format target: formats the main.cpp file
format:
	clang-format src/main.cpp > src/main.cpp

# Build target: checks for inotify-tools, installs if missing, then continuously monitors main.cpp for changes
build:
	mkdir -p build
	@command -v pacman >/dev/null 2>&1 && { sudo pacman -S --noconfirm inotify-tools; } || \
	(command -v apt-get >/dev/null 2>&1 && { sudo apt-get install -y inotify-tools; } || \
	(command -v dnf >/dev/null 2>&1 && { sudo dnf install -y inotify-tools; } || \
	echo "No supported package manager found. Please install inotify-tools manually."))

	cd build && \
	while true; do \
		inotifywait -e modify ../src/main.cpp; \
		make; \
	done