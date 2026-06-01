# Gitea Actions Runner Setup Guide

This directory contains configuration and setup scripts for running Gitea Actions workflows for the string2map project.

## Overview

Gitea Actions is a self-hosted CI/CD solution that runs workflows similar to GitHub Actions. This guide will help you set up self-hosted runners for building and testing the string2map library on Windows and Linux platforms.

## Prerequisites

### General Requirements
- Gitea instance (v1.21.0 or later with Actions enabled)
- Runner registration token from Gitea admin panel
- Internet connectivity for downloading dependencies

### Linux Runner Requirements
- Ubuntu 20.04+ or CentOS 8+
- sudo/root access
- 2+ CPU cores
- 4GB+ RAM
- 20GB+ disk space

**Required packages:**
- curl, tar, git
- cmake (3.29+)
- ninja-build
- build-essential (gcc, g++, clang)
- gcovr (for code coverage)

### Windows Runner Requirements
- Windows 10/Server 2016 or later
- Administrator privileges
- 2+ CPU cores
- 4GB+ RAM
- 20GB+ disk space

**Required software:**
- Visual Studio Build Tools 2022 or Visual Studio Community 2022
- CMake 3.29+
- Ninja
- Git

## Setup Instructions

### Linux Runner Setup

1. **Obtain Registration Token**
   - Log in to your Gitea instance as an administrator
   - Navigate to Site Administration → Actions → Runners
   - Click "Create new runner"
   - Copy the registration token

2. **Run Setup Script**
   ```bash
   # Download the setup script
   curl -O https://your-gitea-instance/path/to/.gitea/setup-runner.sh
   chmod +x setup-runner.sh
   
   # Set environment variables
   export GITEA_URL="https://your-gitea-instance"
   export RUNNER_TOKEN="your-registration-token"
   export RUNNER_NAME="gitea-runner-linux-1"
   export RUNNER_LABELS="linux,ubuntu,x64"
   
   # Run as root
   sudo -E ./setup-runner.sh
   ```

3. **Verify Installation**
   ```bash
   # Check service status
   sudo systemctl status gitea-runner
   
   # View logs
   sudo journalctl -u gitea-runner -f
   ```

4. **Manual Setup (Alternative)**
   ```bash
   # Install dependencies
   sudo apt-get update
   sudo apt-get install -y curl tar git cmake ninja-build build-essential clang gcovr
   
   # Create runner directory
   sudo mkdir -p /opt/gitea-runner
   cd /opt/gitea-runner
   
   # Download runner
   RUNNER_VERSION=$(curl -s https://api.github.com/repos/gitea/act_runner/releases/latest | grep -oP '"tag_name": "\K[^"]+' | sed 's/v//')
   curl -L -o act_runner.tar.gz https://github.com/gitea/act_runner/releases/download/v${RUNNER_VERSION}/act_runner-${RUNNER_VERSION}-linux-amd64.tar.gz
   tar -xzf act_runner.tar.gz
   rm act_runner.tar.gz
   
   # Register runner
   sudo ./act_runner register \
       --instance "https://your-gitea-instance" \
       --token "your-registration-token" \
       --name "gitea-runner-linux-1" \
       --labels "linux,ubuntu,x64" \
       --no-interactive
   
   # Create systemd service
   sudo tee /etc/systemd/system/gitea-runner.service > /dev/null << EOF
   [Unit]
   Description=Gitea Actions Runner
   After=network.target
   
   [Service]
   Type=simple
   User=root
   WorkingDirectory=/opt/gitea-runner
   ExecStart=/opt/gitea-runner/act_runner daemon
   Restart=always
   RestartSec=10
   
   [Install]
   WantedBy=multi-user.target
   EOF
   
   # Enable and start service
   sudo systemctl daemon-reload
   sudo systemctl enable gitea-runner
   sudo systemctl start gitea-runner
   ```

### Windows Runner Setup

1. **Obtain Registration Token**
   - Log in to your Gitea instance as an administrator
   - Navigate to Site Administration → Actions → Runners
   - Click "Create new runner"
   - Copy the registration token

2. **Run Setup Script**
   ```batch
   REM Open Command Prompt as Administrator
   
   REM Download the setup script
   curl -O https://your-gitea-instance/path/to/.gitea/setup-runner.bat
   
   REM Set environment variables (edit the script or set them here)
   set GITEA_URL=https://your-gitea-instance
   set RUNNER_TOKEN=your-registration-token
   set RUNNER_NAME=gitea-runner-windows-1
   set RUNNER_LABELS=windows,x64
   
   REM Run setup script
   setup-runner.bat
   ```

3. **Verify Installation**
   ```batch
   REM Check service status
   sc query gitea-runner
   
   REM View service details
   Get-Service gitea-runner
   ```

4. **Manual Setup (Alternative)**
   ```batch
   REM Open Command Prompt as Administrator
   
   REM Create runner directory
   mkdir C:\gitea-runner
   cd C:\gitea-runner
   
   REM Download runner (use PowerShell)
   powershell -Command "Invoke-WebRequest -Uri 'https://github.com/gitea/act_runner/releases/download/v0.x.x/act_runner-0.x.x-windows-amd64.zip' -OutFile 'act_runner.zip'"
   tar -xf act_runner.zip
   del act_runner.zip
   
   REM Register runner
   act_runner.exe register ^
       --instance "https://your-gitea-instance" ^
       --token "your-registration-token" ^
       --name "gitea-runner-windows-1" ^
       --labels "windows,x64" ^
       --no-interactive
   
   REM Create Windows service
   sc create gitea-runner binPath= "C:\gitea-runner\act_runner.exe daemon" start= auto DisplayName= "Gitea Actions Runner"
   
   REM Start service
   net start gitea-runner
   ```

## Workflow Configuration

The workflow file is located at `.gitea/workflows/build.yml` and defines:

### Build Matrix
- **Windows**: x64 and arm64 architectures with MSVC
- **Linux**: x64 architecture with Clang and GCC
- **Build Types**: Release and Debug

### Jobs
1. **windows-build** - Builds on Windows runners
2. **linux-build** - Builds on Linux runners
3. **publish-nuget** - Publishes to NuGet (main/master only)
4. **publish-gitea** - Creates Gitea releases (main/master only)

### Triggers
- Push to main, master, development, develop, release/* branches
- Pull requests to main and release/* branches
- Manual workflow dispatch

## Runner Labels

Runners should be configured with appropriate labels for job selection:

### Linux Runner Labels
```
linux,ubuntu,x64
```

### Windows Runner Labels
```
windows,x64
```

These labels are used in the workflow to select which runner executes each job.

## Troubleshooting

### Runner Not Connecting
```bash
# Linux: Check logs
sudo journalctl -u gitea-runner -n 50

# Windows: Check Event Viewer
eventvwr.msc
```

### Build Failures
1. Verify all dependencies are installed
2. Check CMake presets are compatible with your system
3. Ensure sufficient disk space (20GB+)
4. Check runner logs for specific error messages

### Service Issues

**Linux:**
```bash
# Restart service
sudo systemctl restart gitea-runner

# Check status
sudo systemctl status gitea-runner

# View detailed logs
sudo journalctl -u gitea-runner -f
```

**Windows:**
```batch
REM Restart service
net stop gitea-runner
net start gitea-runner

REM Check status
sc query gitea-runner
```

## Maintenance

### Updating Runner

**Linux:**
```bash
cd /opt/gitea-runner
sudo systemctl stop gitea-runner

# Download new version
RUNNER_VERSION=$(curl -s https://api.github.com/repos/gitea/act_runner/releases/latest | grep -oP '"tag_name": "\K[^"]+' | sed 's/v//')
curl -L -o act_runner.tar.gz https://github.com/gitea/act_runner/releases/download/v${RUNNER_VERSION}/act_runner-${RUNNER_VERSION}-linux-amd64.tar.gz
tar -xzf act_runner.tar.gz
rm act_runner.tar.gz

sudo systemctl start gitea-runner
```

**Windows:**
```batch
net stop gitea-runner
cd C:\gitea-runner

REM Download new version
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/gitea/act_runner/releases/download/v0.x.x/act_runner-0.x.x-windows-amd64.zip' -OutFile 'act_runner.zip'"
tar -xf act_runner.zip
del act_runner.zip

net start gitea-runner
```

### Cleaning Up

**Linux:**
```bash
# Remove service
sudo systemctl stop gitea-runner
sudo systemctl disable gitea-runner
sudo rm /etc/systemd/system/gitea-runner.service
sudo systemctl daemon-reload

# Remove runner directory
sudo rm -rf /opt/gitea-runner
```

**Windows:**
```batch
REM Remove service
net stop gitea-runner
sc delete gitea-runner

REM Remove runner directory
rmdir /s /q C:\gitea-runner
```

## Security Considerations

1. **Runner Token**: Keep registration tokens secure and rotate regularly
2. **Secrets**: Use Gitea repository secrets for sensitive data (API keys, credentials)
3. **Network**: Ensure runners are on a secure network
4. **Updates**: Keep runners and dependencies updated
5. **Permissions**: Run runners with minimal required privileges

## Additional Resources

- [Gitea Actions Documentation](https://docs.gitea.io/en-us/actions/)
- [Act Runner GitHub Repository](https://github.com/gitea/act_runner)
- [CMake Documentation](https://cmake.org/documentation/)
- [Ninja Build System](https://ninja-build.org/)

## Support

For issues or questions:
1. Check the troubleshooting section above
2. Review runner logs
3. Consult Gitea Actions documentation
4. Open an issue in the project repository
