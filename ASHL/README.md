## Usage

```ruby
Usage: [Root] ./SecurityCheckup.sh [OPTIONS]

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
