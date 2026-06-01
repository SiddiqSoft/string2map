# Gitea Actions Quick Reference

## Quick Start

### Linux Runner (One-liner)
```bash
curl -fsSL https://your-gitea-instance/path/to/.gitea/setup-runner.sh | \
  GITEA_URL="https://your-gitea-instance" \
  RUNNER_TOKEN="your-token" \
  RUNNER_NAME="gitea-runner-linux-1" \
  RUNNER_LABELS="linux,ubuntu,x64" \
  sudo bash
```

### Windows Runner (PowerShell)
```powershell
# Run as Administrator
$env:GITEA_URL = "https://your-gitea-instance"
$env:RUNNER_TOKEN = "your-token"
$env:RUNNER_NAME = "gitea-runner-windows-1"
$env:RUNNER_LABELS = "windows,x64"

# Download and run
Invoke-WebRequest -Uri "https://your-gitea-instance/path/to/.gitea/setup-runner.bat" -OutFile "setup-runner.bat"
.\setup-runner.bat
```

## Common Commands

### Linux

```bash
# Check runner status
sudo systemctl status gitea-runner

# View logs (last 50 lines)
sudo journalctl -u gitea-runner -n 50

# View logs in real-time
sudo journalctl -u gitea-runner -f

# Restart runner
sudo systemctl restart gitea-runner

# Stop runner
sudo systemctl stop gitea-runner

# Start runner
sudo systemctl start gitea-runner

# View runner configuration
cat /opt/gitea-runner/.runner
```

### Windows

```batch
REM Check runner status
sc query gitea-runner

REM View service details
Get-Service gitea-runner

REM Restart runner
net stop gitea-runner
net start gitea-runner

REM View runner configuration
type C:\gitea-runner\.runner
```

## Workflow Triggers

The workflow is triggered by:
- Push to `main`, `master`, `development`, `develop`, `release/*` branches
- Pull requests to `main` and `release/*` branches
- Manual workflow dispatch

## Build Matrix

| Platform | Architecture | Compiler | Build Type |
|----------|--------------|----------|-----------|
| Windows  | x64          | MSVC     | Release   |
| Windows  | arm64        | MSVC     | Release   |
| Linux    | x64          | Clang    | Release   |
| Linux    | x64          | Clang    | Debug     |
| Linux    | x64          | GCC      | Release   |
| Linux    | x64          | GCC      | Debug     |

## Environment Variables

### Setup Script Variables

```bash
# Linux
export GITEA_URL="https://your-gitea-instance"
export RUNNER_TOKEN="your-registration-token"
export RUNNER_NAME="gitea-runner-linux-1"
export RUNNER_LABELS="linux,ubuntu,x64"
export RUNNER_DIR="/opt/gitea-runner"
export RUNNER_USER="gitea-runner"
export RUNNER_GROUP="gitea-runner"

# Windows
set GITEA_URL=https://your-gitea-instance
set RUNNER_TOKEN=your-registration-token
set RUNNER_NAME=gitea-runner-windows-1
set RUNNER_LABELS=windows,x64
set RUNNER_DIR=C:\gitea-runner
```

### Workflow Secrets

Configure these in Gitea repository settings:
- `NUGET_API_KEY` - NuGet API key for publishing
- `GITEA_TOKEN` - Gitea API token for releases

## Troubleshooting

### Runner won't start

**Linux:**
```bash
# Check if port is in use
sudo lsof -i :3000

# Check systemd errors
sudo systemctl status gitea-runner
sudo journalctl -u gitea-runner -n 100

# Manually test runner
cd /opt/gitea-runner
./act_runner daemon
```

**Windows:**
```batch
REM Check service status
sc query gitea-runner

REM Check if runner can connect
cd C:\gitea-runner
act_runner.exe daemon
```

### Build failures

1. Check runner logs for specific errors
2. Verify all dependencies are installed
3. Ensure sufficient disk space
4. Check CMake preset compatibility

### Connection issues

1. Verify Gitea URL is correct and accessible
2. Check runner token is valid
3. Verify network connectivity
4. Check firewall rules

## File Locations

| Item | Linux | Windows |
|------|-------|---------|
| Runner Directory | `/opt/gitea-runner` | `C:\gitea-runner` |
| Configuration | `/opt/gitea-runner/.runner` | `C:\gitea-runner\.runner` |
| Logs | `journalctl -u gitea-runner` | Event Viewer |
| Service | `gitea-runner.service` | `gitea-runner` (Windows Service) |

## Performance Tuning

### Linux
```bash
# Increase file descriptor limit
sudo sysctl -w fs.file-max=2097152

# Increase process limit
sudo sysctl -w kernel.pid_max=4194303

# Make persistent
echo "fs.file-max=2097152" | sudo tee -a /etc/sysctl.conf
echo "kernel.pid_max=4194303" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

### Windows
```batch
REM Increase resource limits in service configuration
REM Edit service properties or use Group Policy
```

## Docker Testing

```bash
# Start local Gitea with runner
docker-compose -f .gitea/docker-compose.yml up -d

# View logs
docker-compose -f .gitea/docker-compose.yml logs -f

# Stop services
docker-compose -f .gitea/docker-compose.yml down

# Access Gitea
# http://localhost:3000
# Default: gitea / gitea
```

## Useful Links

- [Gitea Actions Docs](https://docs.gitea.io/en-us/actions/)
- [Act Runner GitHub](https://github.com/gitea/act_runner)
- [CMake Documentation](https://cmake.org/documentation/)
- [Ninja Build System](https://ninja-build.org/)
- [NuGet Documentation](https://docs.microsoft.com/en-us/nuget/)

## Support

For issues:
1. Check logs (see commands above)
2. Review README.md for detailed setup
3. Check Gitea Actions documentation
4. Open issue in project repository
