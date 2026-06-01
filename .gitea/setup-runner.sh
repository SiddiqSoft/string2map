#!/bin/bash
# ============================================================================
# Gitea Actions Runner Setup Script
# ============================================================================
#
# This script sets up a Gitea Actions runner on a Linux system.
# It installs the runner, configures it, and sets it up as a systemd service.
#
# Requirements:
# - Linux system (Ubuntu 20.04+, CentOS 8+, etc.)
# - sudo access
# - Internet connection
#
# Usage:
#   chmod +x setup-gitea-runner.sh
#   ./setup-gitea-runner.sh
#
# Configuration:
#   Edit the variables below to match your Gitea instance and runner setup
#
# ============================================================================

set -e

# ============================================================================
# CONFIGURATION
# ============================================================================

# Gitea instance URL
GITEA_URL="${GITEA_URL:-https://gitea.example.com}"

# Runner registration token (obtain from Gitea admin panel)
RUNNER_TOKEN="${RUNNER_TOKEN:-}"

# Runner name (should be unique)
RUNNER_NAME="${RUNNER_NAME:-gitea-runner-linux-$(hostname)}"

# Runner labels (comma-separated)
RUNNER_LABELS="${RUNNER_LABELS:-linux,ubuntu,x64}"

# Installation directory
RUNNER_DIR="${RUNNER_DIR:-/opt/gitea-runner}"

# Runner user (will be created if doesn't exist)
RUNNER_USER="${RUNNER_USER:-gitea-runner}"

# Runner group
RUNNER_GROUP="${RUNNER_GROUP:-gitea-runner}"

# ============================================================================
# FUNCTIONS
# ============================================================================

log_info() {
    echo "[INFO] $1"
}

log_error() {
    echo "[ERROR] $1" >&2
}

log_success() {
    echo "[SUCCESS] $1"
}

check_requirements() {
    log_info "Checking system requirements..."
    
    # Check if running as root
    if [[ $EUID -ne 0 ]]; then
        log_error "This script must be run as root"
        exit 1
    fi
    
    # Check for required commands
    local required_commands=("curl" "tar" "git")
    for cmd in "${required_commands[@]}"; do
        if ! command -v "$cmd" &> /dev/null; then
            log_error "Required command not found: $cmd"
            exit 1
        fi
    done
    
    log_success "System requirements met"
}

install_dependencies() {
    log_info "Installing dependencies..."
    
    # Detect OS
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        OS=$ID
    else
        log_error "Cannot detect OS"
        exit 1
    fi
    
    case "$OS" in
        ubuntu|debian)
            apt-get update
            apt-get install -y \
                curl \
                tar \
                git \
                cmake \
                ninja-build \
                build-essential \
                clang \
                gcc \
                g++ \
                gcovr
            ;;
        centos|rhel|fedora)
            yum install -y \
                curl \
                tar \
                git \
                cmake \
                ninja-build \
                gcc \
                gcc-c++ \
                clang \
                gcovr
            ;;
        *)
            log_error "Unsupported OS: $OS"
            exit 1
            ;;
    esac
    
    log_success "Dependencies installed"
}

create_runner_user() {
    log_info "Creating runner user..."
    
    if id "$RUNNER_USER" &>/dev/null; then
        log_info "User $RUNNER_USER already exists"
    else
        useradd -m -s /bin/bash "$RUNNER_USER"
        log_success "User $RUNNER_USER created"
    fi
}

download_runner() {
    log_info "Downloading Gitea Actions runner..."
    
    # Detect architecture
    ARCH=$(uname -m)
    case "$ARCH" in
        x86_64)
            RUNNER_ARCH="amd64"
            ;;
        aarch64)
            RUNNER_ARCH="arm64"
            ;;
        *)
            log_error "Unsupported architecture: $ARCH"
            exit 1
            ;;
    esac
    
    # Get latest runner version
    RUNNER_VERSION=$(curl -s "https://api.github.com/repos/gitea/act_runner/releases/latest" | grep -oP '"tag_name": "\K[^"]+')
    RUNNER_VERSION=${RUNNER_VERSION#v}
    
    if [ -z "$RUNNER_VERSION" ]; then
        log_error "Could not determine runner version"
        exit 1
    fi
    
    log_info "Latest runner version: $RUNNER_VERSION"
    
    # Create installation directory
    mkdir -p "$RUNNER_DIR"
    cd "$RUNNER_DIR"
    
    # Download runner
    RUNNER_URL="https://github.com/gitea/act_runner/releases/download/v${RUNNER_VERSION}/act_runner-${RUNNER_VERSION}-linux-${RUNNER_ARCH}.tar.gz"
    log_info "Downloading from: $RUNNER_URL"
    
    curl -L -o act_runner.tar.gz "$RUNNER_URL"
    tar -xzf act_runner.tar.gz
    rm act_runner.tar.gz
    
    chmod +x act_runner
    
    log_success "Runner downloaded and extracted"
}

register_runner() {
    log_info "Registering runner with Gitea..."
    
    if [ -z "$RUNNER_TOKEN" ]; then
        log_error "RUNNER_TOKEN not set. Please obtain a registration token from Gitea admin panel."
        exit 1
    fi
    
    cd "$RUNNER_DIR"
    
    # Register runner
    ./act_runner register \
        --instance "$GITEA_URL" \
        --token "$RUNNER_TOKEN" \
        --name "$RUNNER_NAME" \
        --labels "$RUNNER_LABELS" \
        --no-interactive
    
    log_success "Runner registered"
}

setup_systemd_service() {
    log_info "Setting up systemd service..."
    
    # Create systemd service file
    cat > /etc/systemd/system/gitea-runner.service << EOF
[Unit]
Description=Gitea Actions Runner
After=network.target

[Service]
Type=simple
User=$RUNNER_USER
WorkingDirectory=$RUNNER_DIR
ExecStart=$RUNNER_DIR/act_runner daemon
Restart=always
RestartSec=10

# Security settings
NoNewPrivileges=true
PrivateTmp=true

# Resource limits
LimitNOFILE=65536
LimitNPROC=65536

[Install]
WantedBy=multi-user.target
EOF
    
    # Set proper permissions
    chown -R "$RUNNER_USER:$RUNNER_GROUP" "$RUNNER_DIR"
    chmod 755 "$RUNNER_DIR"
    chmod 755 "$RUNNER_DIR/act_runner"
    
    # Reload systemd
    systemctl daemon-reload
    
    log_success "Systemd service created"
}

enable_and_start_service() {
    log_info "Enabling and starting service..."
    
    systemctl enable gitea-runner
    systemctl start gitea-runner
    
    # Wait a moment for service to start
    sleep 2
    
    # Check service status
    if systemctl is-active --quiet gitea-runner; then
        log_success "Service started successfully"
    else
        log_error "Service failed to start"
        systemctl status gitea-runner
        exit 1
    fi
}

verify_installation() {
    log_info "Verifying installation..."
    
    # Check if runner is registered
    if [ -f "$RUNNER_DIR/.runner" ]; then
        log_success "Runner configuration found"
    else
        log_error "Runner configuration not found"
        exit 1
    fi
    
    # Check service status
    systemctl status gitea-runner
}

# ============================================================================
# MAIN
# ============================================================================

main() {
    log_info "Starting Gitea Actions Runner setup..."
    log_info "Configuration:"
    log_info "  Gitea URL: $GITEA_URL"
    log_info "  Runner Name: $RUNNER_NAME"
    log_info "  Runner Labels: $RUNNER_LABELS"
    log_info "  Installation Directory: $RUNNER_DIR"
    log_info "  Runner User: $RUNNER_USER"
    
    check_requirements
    install_dependencies
    create_runner_user
    download_runner
    register_runner
    setup_systemd_service
    enable_and_start_service
    verify_installation
    
    log_success "Gitea Actions Runner setup completed!"
    log_info "Service status: $(systemctl is-active gitea-runner)"
    log_info "View logs with: journalctl -u gitea-runner -f"
}

main "$@"
