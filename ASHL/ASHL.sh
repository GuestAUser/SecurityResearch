#!/usr/bin/env bash
# All-in-one security & health checker for Linux - ASHL
# GuestAUser @ GitHub
# (1.75.0)

set -euo pipefail

##############################################################################
# GLOBAL OPTIONS & COLORS ***************************************************|
##############################################################################

VERBOSE=0
AUTO_INSTALL=0
STEP_PROMPT=0

SKIP_ROOTKIT=0
SKIP_CLAMAV=0
SKIP_LYNIS=0
SKIP_SMART=0
SKIP_FIREWALL=0
SKIP_UPDATE=0
SKIP_SYSHEALTH=0

GREEN="\e[32m"
YELLOW="\e[33m"
RED="\e[31m"
CYAN="\e[36m"
RESET="\e[0m"

PKG_INSTALL_CMD=""
PKG_UPDATE_CMD=""
PKG_CHECK_CMD=""

##############################################################################
# LOGGING & HELPER FUNCTIONS ************************************************|
##############################################################################

info()  { [ "$VERBOSE" -eq 1 ] && echo -e "${GREEN}[INFO]${RESET}  $*"; }
warn()  { echo -e "${YELLOW}[WARN]${RESET}  $*"; }
error() { echo -e "${RED}[ERROR]${RESET} $*" >&2; }

pause_step() {
  if [ "$STEP_PROMPT" -eq 1 ]; then
    read -r -p "Press Enter to continue..."
  fi
}

##############################################################################
# ASCII BANNER **************************************************************|
##############################################################################

display_ascii_art() {
  echo -e "${CYAN}"
  cat << 'EOF'
        .___.
       /     \
      | O _ O |
      /  \_/  \
    .' /     \ `.
   / _|       |_ \
  (_/ |       | \_)
      \       /
     __\_>-<_/__   Linux S&H Tool - Davi Chaves Azevedo
     ~;/     \;~

EOF
  echo -e "${RESET}"
  echo -e "${CYAN}=== The Omnipotent Linux Security & Health Checker ===${RESET}"
  echo
}

##############################################################################
# USAGE & ARGUMENT PARSING **************************************************|
##############################################################################

usage() {
  cat <<EOF
Usage: [ROOT] $0 [OPTIONS]

Options:
  -v, --verbose       Show detailed/verbose output for each step.
  -y, --yes           Auto-install missing dependencies (no interactive prompt).
  -h, --help          Show this help message.
  --step-pause        Pause after each major step (press Enter to continue).
  --skip-rootkit      Skip rootkit checks (rkhunter).
  --skip-clamav       Skip ClamAV updates & scanning.
  --skip-lynis        Skip Lynis security audit.
  --skip-smart        Skip S.M.A.R.T. disk checks.
  --skip-firewall     Skip firewall status check.
  --skip-updates      Skip system update check.
  --skip-syshealth    Skip system health overview.

Examples:
  sudo $0 -v
  sudo $0 --skip-rootkit --skip-updates
  sudo $0 --step-pause (will pause after each step)

EOF
  exit 0
}

[[ $EUID -ne 0 ]] && { error "Please run as root."; exit 1; }

while [[ $# -gt 0 ]]; do
  case "$1" in
    -v|--verbose)     VERBOSE=1 ;;
    -y|--yes)         AUTO_INSTALL=1 ;;
    -h|--help)        usage ;;
    --step-pause)     STEP_PROMPT=1 ;;
    --skip-rootkit)   SKIP_ROOTKIT=1 ;;
    --skip-clamav)    SKIP_CLAMAV=1 ;;
    --skip-lynis)     SKIP_LYNIS=1 ;;
    --skip-smart)     SKIP_SMART=1 ;;
    --skip-firewall)  SKIP_FIREWALL=1 ;;
    --skip-updates)   SKIP_UPDATE=1 ;;
    --skip-syshealth) SKIP_SYSHEALTH=1 ;;
    *)
      error "Unknown option: $1"
      usage
      ;;
  esac
  shift
done

##############################################################################
# DETECT PACKAGE MANAGER ****************************************************|
##############################################################################

detect_package_manager() {
  if   command -v apt-get &>/dev/null; then
    PKG_INSTALL_CMD="apt-get install -y"
    PKG_UPDATE_CMD="apt-get update"
    PKG_CHECK_CMD="dpkg -s"
  elif command -v pacman &>/dev/null; then
    PKG_INSTALL_CMD="pacman --noconfirm -Sy"
    PKG_UPDATE_CMD="pacman -Sy"
    PKG_CHECK_CMD="pacman -Q"
  elif command -v dnf &>/dev/null; then
    PKG_INSTALL_CMD="dnf install -y"
    PKG_UPDATE_CMD="dnf check-update || true"
    PKG_CHECK_CMD="dnf list installed"
  elif command -v zypper &>/dev/null; then
    PKG_INSTALL_CMD="zypper install -y"
    PKG_UPDATE_CMD="zypper refresh"
    PKG_CHECK_CMD="zypper se --installed-only"
  else
    error "No supported package manager found (apt, pacman, dnf, zypper)."
    exit 1
  fi
}

detect_package_manager

##############################################################################
# DEPENDENCY CHECK & INSTALL ************************************************|
##############################################################################

install_if_missing() {
  local pkg="$1" bin="$2"

  if command -v "$bin" &>/dev/null; then
    info "'$bin' is already available; skipping install of '$pkg'."
    return
  fi

  if $PKG_CHECK_CMD "$pkg" &>/dev/null; then
    info "'$pkg' is installed, but '$bin' not in PATH. Check PATH or reinstall."
    return
  fi

  warn "Missing dependency: $pkg ($bin)."

  if [ "$AUTO_INSTALL" -eq 1 ]; then
    info "Auto-installing $pkg..."
    $PKG_UPDATE_CMD || warn "Update command failed (not critical)."
    $PKG_INSTALL_CMD "$pkg" || warn "Could not install $pkg."
  else
    read -r -p "Install $pkg now? (Y/n): " answer
    if [[ "$answer" =~ ^([yY]|[yY][eE][sS]|'')$ ]]; then
      $PKG_UPDATE_CMD || warn "Update command failed (not critical)."
      $PKG_INSTALL_CMD "$pkg" || warn "Could not install $pkg."
    else
      warn "Skipping $pkg; some features may not work."
    fi
  fi
}

check_dependencies() {
  install_if_missing rkhunter      rkhunter
  install_if_missing clamav        clamscan
  install_if_missing lynis         lynis
  install_if_missing smartmontools smartctl
}

##############################################################################
# MAIN FUNCTIONS ************************************************************|
##############################################################################

system_health() {
  echo -e "\n${CYAN}==== SYSTEM HEALTH ====${RESET}"
  echo "Uptime:       $(uptime -p 2>/dev/null || true)"
  echo -e "\nDisk Usage:"
  df -h || true

  echo -e "\nMemory Usage:"
  free -h || true

  echo -e "\nLoad Average:"
  uptime 2>/dev/null | awk -F'load average:' '{print $2}' || true

  echo -e "\nOpen Ports:"
  if command -v ss &>/dev/null; then
    ss -tuln || true
  else
    netstat -tuln 2>/dev/null || true
  fi
  echo
}

update_check() {
  info "Checking for system updates..."
  if command -v apt-get &>/dev/null; then
    apt-get update || true
    local upg
    upg="$(apt-get --just-print upgrade 2>/dev/null | grep '^Inst ' || true)"
    if [ -n "$upg" ]; then
      echo "Updates available:"
      echo "$upg"
    else
      echo "System is up-to-date."
    fi
  elif command -v pacman &>/dev/null; then
    pacman -Sy || true
    local updates
    updates="$(pacman -Qu 2>/dev/null || true)"
    if [ -n "$updates" ]; then
      echo "Updates available:"
      echo "$updates"
    else
      echo "System is up-to-date."
    fi
  elif command -v dnf &>/dev/null; then
    dnf check-update || true
  elif command -v zypper &>/dev/null; then
    zypper refresh || true
    zypper list-updates || true
  fi
  echo
}

firewall_check() {
  info "Checking firewall status..."
  if command -v ufw &>/dev/null; then
    ufw status || warn "ufw status command failed."
  elif command -v iptables &>/dev/null; then
    iptables -L -n || true
  elif command -v nft &>/dev/null; then
    nft list ruleset || true
  else
    warn "No known firewall (ufw/iptables/nft) found; skipping."
  fi
  echo
}

rootkit_checks() {
  info "Running rkhunter non-interactively..."
  if command -v rkhunter &>/dev/null; then
    rkhunter --update --nocolors --sk --cronjob || warn "rkhunter update warnings."
    rkhunter --check  --nocolors --sk --cronjob || warn "rkhunter check warnings."
  else
    warn "rkhunter not found; skipping rootkit checks."
  fi
  echo
}

clamav_scan() {
  echo -e "${RED}WARNING: A full ClamAV scan can be time-consuming on large directories!${RESET}"
  read -r -p "Do you want to continue with the ClamAV scan? (Y/n): " resp
  if [[ "$resp" =~ ^([nN]|[nN][oO])$ ]]; then
    warn "Skipping ClamAV scan by user request."
    return
  fi

  info "Updating ClamAV definitions..."
  if command -v freshclam &>/dev/null; then
    if [ "$VERBOSE" -eq 1 ]; then
      freshclam --verbose || warn "freshclam update failed."
    else
      freshclam || warn "freshclam update failed."
    fi
  else
    warn "freshclam not found; skipping ClamAV definition update."
  fi

  info "Scanning /home and /etc with ClamAV..."
  if command -v clamscan &>/dev/null; then
    if [ "$VERBOSE" -eq 1 ]; then
      clamscan -r -i -v /home /etc || warn "clamscan encountered an issue."
    else
      clamscan -r -i /home /etc || warn "clamscan encountered an issue."
    fi
  else
    warn "clamscan not found; skipping virus scan."
  fi
  echo
}

lynis_audit() {
  info "Running Lynis audit (non-interactive)..."
  if command -v lynis &>/dev/null; then
    lynis audit system --quick || warn "Lynis encountered warnings."
  else
    warn "lynis not found; skipping."
  fi
  echo
}

smart_check() {
  info "Running S.M.A.R.T. disk checks..."
  if ! command -v smartctl &>/dev/null; then
    warn "smartctl not found; skipping disk checks."
    return
  fi

  mapfile -t disks < <(lsblk -dno NAME 2>/dev/null || true)
  for d in "${disks[@]}"; do
    [ -b "/dev/$d" ] || continue
    echo "---- /dev/$d ----"
    smartctl -H "/dev/$d" 2>/dev/null | grep -E 'overall-health|overall health|test result' || true
    smartctl -A "/dev/$d" 2>/dev/null | grep -i "temperature" || true
    echo
  done
}

##############################################################################
# MAIN EXECUTION ************************************************************|
##############################################################################

display_ascii_art
check_dependencies
pause_step

if [ "$SKIP_SYSHEALTH" -eq 0 ]; then
  system_health
  pause_step
fi

if [ "$SKIP_UPDATE" -eq 0 ]; then
  update_check
  pause_step
fi

if [ "$SKIP_FIREWALL" -eq 0 ]; then
  firewall_check
  pause_step
fi

if [ "$SKIP_ROOTKIT" -eq 0 ]; then
  rootkit_checks
  pause_step
fi

if [ "$SKIP_CLAMAV" -eq 0 ]; then
  clamav_scan
  pause_step
fi

if [ "$SKIP_LYNIS" -eq 0 ]; then
  lynis_audit
  pause_step
fi

if [ "$SKIP_SMART" -eq 0 ]; then
  smart_check
  pause_step
fi

echo -e "${GREEN}All requested checks completed.${RESET}"
