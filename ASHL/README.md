## Usage

```ruby
Usage: [Root] ./ASHL.sh [OPTIONS]

Options:
  -v, --verbose       Show detailed/verbose output for each step.
  -y, --yes           Auto-install missing dependencies (no interactive prompt).
  -h, --help          Show this help message.

  --skip-rootkit      Skip rootkit checks (rkhunter, chkrootkit).
  --skip-clamav       Skip ClamAV updates & scanning.
  --skip-lynis        Skip Lynis security audit.
  --skip-smart        Skip S.M.A.R.T. disk checks.
  --skip-firewall     Skip firewall status check.
  --skip-updates      Skip system update check.
  --skip-syshealth    Skip system health overview.

Examples:
  sudo ./SecurityCheckup.sh -v
  sudo ./SecurityCheckup.sh --skip-rootkit --skip-updates
```

# ASHL — All-in-One Linux Security & Health Checker

> **Version:** 1.0  
> **Author:** [GuestAUser @ GitHub](https://github.com/GuestAUser)  
> **License:** MIT  

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
  - [Basic Examples](#basic-examples)
  - [Options](#options)
- [What It Does](#what-it-does)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

**ASHL** *(All-in-One Security & Health Checker for Linux)*, a comprehensive bash script that quickly gathers key system health information and performs fundamental security checks. Designed for flexibility and simplicity, it detects common package managers to install necessary tools, or it can prompt for manual installation, ensuring a painless setup across major **Linux** distributions.

---

## Features

- **System Health Overview**: Displays uptime, disk usage, memory usage, load average, and open ports.
- **Package Update Check**: Checks for available system package updates.
- **Firewall Status**: Scans for `ufw`, `iptables`, or `nft` status.
- **Rootkit Detection**: Uses `rkhunter` for scanning potential rootkits (optionally installs if missing).
- **Antivirus Check**: Offers ClamAV scanning with automatic definition updates.
- **Security Audit**: Uses `lynis` for a quick system hardening review.
- **S.M.A.R.T. Disk Analysis**: Uses `smartmontools` to check disk health (temperature, overall status).
- **Automatic or Interactive**: Can auto-install missing dependencies (`-y`) or prompt the user for each.
- **Skip Options**: Choose which checks to skip to save time or resources.

---

## Requirements

1. **Root Privileges**: Must be run as `root` (or via `sudo`).  
2. **Supported Package Managers**:  
   - `apt-get` (Debian/Ubuntu)
   - `pacman` (Arch/Manjaro)
   - `dnf` (Fedora/CentOS)
   - `zypper` (openSUSE)

If none of these are available, the script will warn and exit.

---

## Installation

1. **Download the Script**  
   Clone the repository or just grab the `ashl.sh` file:
   ```bash
   git clone https://github.com/GuestAUser/SecurityResearch/tree/main/ASHL
   cd ASHL
   ```
2. **Make It Executable**  
   ```bash
   chmod +x ASHL.sh
   ```
3. **Run It as Root**  
   ```bash
   sudo ./ASHL.sh
   ```
   Or move `ASHL.sh` to your PATH (e.g. `/usr/local/bin/ASHL`) for system-wide usage.

---

## Usage

```bash
sudo ./ASHL.sh [OPTIONS]
```

### Basic Examples

- **Run All Checks Verbosely**:
  ```bash
  sudo ./ASHL.sh -v
  ```
- **Skip Only Rootkit Checks**:
  ```bash
  sudo ./ASHL.sh --skip-rootkit
  ```
- **Auto-Install Dependencies Without Prompts**:
  ```bash
  sudo ./ASHL.sh -y
  ```

### Options

| Option            | Description                                                               |
|-------------------|---------------------------------------------------------------------------|
| `-v, --verbose`   | Show detailed/verbose output for each step.                               |
| `-y, --yes`       | Auto-install missing dependencies (no interactive prompts).               |
| `-h, --help`      | Display help/usage information and exit.                                  |
| `--skip-rootkit`  | Skip rootkit checks (via `rkhunter`).                                     |
| `--skip-clamav`   | Skip ClamAV antivirus checks and definition updates.                      |
| `--skip-lynis`    | Skip `lynis` security audit.                                              |
| `--skip-smart`    | Skip S.M.A.R.T. disk checks (`smartctl`).                                 |
| `--skip-firewall` | Skip firewall status checks (`ufw` / `iptables` / `nft`).                 |
| `--skip-updates`  | Skip system package update checks.                                        |
| `--skip-syshealth`| Skip general system health overview.                                      |

---

## What It Does

1. **Dependency Checks**  
   - Detects if `rkhunter`, `clamav`, `lynis`, and `smartmontools` are installed.  
   - Prompts to install or auto-installs them if invoked with `-y`.  
2. **System Health** (skippable with `--skip-syshealth`)  
   - Displays uptime, disk usage, memory usage, load average, and open ports.  
3. **Update Check** (skippable with `--skip-updates`)  
   - Checks for available updates using your distribution’s package manager.  
4. **Firewall Check** (skippable with `--skip-firewall`)  
   - Displays `ufw`, `iptables`, or `nft` status, if installed.  
5. **Rootkit Detection** (skippable with `--skip-rootkit`)  
   - Uses `rkhunter` to scan the system for potential rootkits.  
6. **ClamAV Antivirus Check** (skippable with `--skip-clamav`)  
   - Updates ClamAV definitions (via `freshclam`) and scans `/home` and `/etc`.  
7. **Lynis Security Audit** (skippable with `--skip-lynis`)  
   - Runs `lynis audit system --quick` to provide security recommendations.  
8. **S.M.A.R.T. Disk Check** (skippable with `--skip-smart`)  
   - Uses `smartctl` to show disk health and temperature info.  

---

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests for enhancements, bug fixes, or additional features. Please follow a clear commit message format and provide detailed information in pull requests.

---

## License

This project is available under the [MIT License](https://github.com/GuestAUser/SecurityResearch/blob/780ce04b71810fc23b61a73f3370b06d967a3659/LICENSE). You’re free to modify, distribute, and use it in private or commercial environments. If you make significant improvements, consider contributing them back to the community!
