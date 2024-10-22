![Logo](assets/newlinux-rounded-small.png)
# NewLinux

![License](https://img.shields.io/badge/license-GPL%203.0-blue.svg)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Version](https://img.shields.io/badge/version-1.0.0-orange.svg)

NewLinux is a kernel update tool designed to simplify the process of updating your Linux kernel. With a focus on reliability and user-friendliness, NewLinux provides a range of features to help you manage kernel updates effectively.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Features

- Simple and intuitive interface
- Automatic kernel version detection
- Backup and restore functionality
- Live watching for source file changes with `make build`

## Installation

To install NewLinux, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/voltageddebunked/NewLinux.git
   cd NewLinux
   ```

2. Build the project:

   ```bash
   make
   ```

3. Install the application:

   ```bash
   sudo make install
   ```

4. Format the code:

   ```bash
   make format
   ```

5. Start watching for changes in `src/main.cpp`:

   ```bash
   make build
   ```

**NOTE: It is better to run as administrator, since if you dont have a terminal and run it, you wont be able to confirm installation. With admin it confirms automatically.**

## Usage

After installation, you can launch NewLinux from your applications menu or by running the following command in your terminal:

```bash
newlinux
```

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss potential changes.

## License

NewLinux is licensed under the GPL 3.0 License. See the [LICENSE](LICENSE) file for more information.

## Contact

For any inquiries, please contact:
[Voltaged](mailto:rusindanilo@gmail.com)
[GitHub](https://github.com/voltageddebunked)
