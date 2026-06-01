@echo off
REM ============================================================================
REM Gitea Actions Runner Setup Script for Windows
REM ============================================================================
REM
REM This script sets up a Gitea Actions runner on a Windows system.
REM It downloads the runner, configures it, and sets it up as a Windows service.
REM
REM Requirements:
REM - Windows 10/Server 2016 or later
REM - Administrator privileges
REM - Internet connection
REM - Visual Studio Build Tools or Visual Studio Community
REM
REM Usage:
REM   setup-gitea-runner.bat
REM
REM Configuration:
REM   Edit the variables below to match your Gitea instance and runner setup
REM
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION
REM ============================================================================

REM Gitea instance URL
set "GITEA_URL=https://gitea.example.com"

REM Runner registration token (obtain from Gitea admin panel)
set "RUNNER_TOKEN="

REM Runner name (should be unique)
for /f "tokens=*" %%A in ('hostname') do set "HOSTNAME=%%A"
set "RUNNER_NAME=gitea-runner-windows-%HOSTNAME%"

REM Runner labels (comma-separated)
set "RUNNER_LABELS=windows,x64"

REM Installation directory
set "RUNNER_DIR=C:\gitea-runner"

REM ============================================================================
REM FUNCTIONS
REM ============================================================================

:log_info
echo [INFO] %~1
exit /b 0

:log_error
echo [ERROR] %~1 1>&2
exit /b 1

:log_success
echo [SUCCESS] %~1
exit /b 0

:check_admin
REM Check if running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    call :log_error "This script must be run as Administrator"
    exit /b 1
)
exit /b 0

:check_requirements
call :log_info "Checking system requirements..."

REM Check for required commands
for %%A in (curl.exe tar.exe git.exe) do (
    where %%A >nul 2>&1
    if errorlevel 1 (
        call :log_error "Required command not found: %%A"
        exit /b 1
    )
)

call :log_success "System requirements met"
exit /b 0

:install_dependencies
call :log_info "Installing dependencies..."

REM Check for CMake
where cmake >nul 2>&1
if errorlevel 1 (
    call :log_info "CMake not found. Please install CMake from https://cmake.org/download/"
)

REM Check for Ninja
where ninja >nul 2>&1
if errorlevel 1 (
    call :log_info "Ninja not found. Please install Ninja from https://github.com/ninja-build/ninja/releases"
)

REM Check for Visual Studio
where cl.exe >nul 2>&1
if errorlevel 1 (
    call :log_info "Visual Studio not found. Please install Visual Studio Build Tools"
)

call :log_success "Dependencies check completed"
exit /b 0

:download_runner
call :log_info "Downloading Gitea Actions runner..."

REM Detect architecture
set "RUNNER_ARCH=amd64"
if "%PROCESSOR_ARCHITECTURE%"=="ARM64" set "RUNNER_ARCH=arm64"

REM Get latest runner version from GitHub API
for /f "tokens=*" %%A in ('powershell -Command "(Invoke-WebRequest -Uri 'https://api.github.com/repos/gitea/act_runner/releases/latest' -UseBasicParsing | ConvertFrom-Json).tag_name" 2^>nul') do set "RUNNER_VERSION=%%A"

if "!RUNNER_VERSION!"=="" (
    call :log_error "Could not determine runner version"
    exit /b 1
)

set "RUNNER_VERSION=!RUNNER_VERSION:v=!"
call :log_info "Latest runner version: !RUNNER_VERSION!"

REM Create installation directory
if not exist "!RUNNER_DIR!" mkdir "!RUNNER_DIR!"
cd /d "!RUNNER_DIR!"

REM Download runner
set "RUNNER_URL=https://github.com/gitea/act_runner/releases/download/v!RUNNER_VERSION!/act_runner-!RUNNER_VERSION!-windows-!RUNNER_ARCH!.zip"
call :log_info "Downloading from: !RUNNER_URL!"

powershell -Command "Invoke-WebRequest -Uri '!RUNNER_URL!' -OutFile 'act_runner.zip'" 2>nul
if errorlevel 1 (
    call :log_error "Failed to download runner"
    exit /b 1
)

REM Extract runner
tar -xf act_runner.zip
del act_runner.zip

call :log_success "Runner downloaded and extracted"
exit /b 0

:register_runner
call :log_info "Registering runner with Gitea..."

if "!RUNNER_TOKEN!"=="" (
    call :log_error "RUNNER_TOKEN not set. Please obtain a registration token from Gitea admin panel."
    exit /b 1
)

cd /d "!RUNNER_DIR!"

REM Register runner
act_runner.exe register ^
    --instance "!GITEA_URL!" ^
    --token "!RUNNER_TOKEN!" ^
    --name "!RUNNER_NAME!" ^
    --labels "!RUNNER_LABELS!" ^
    --no-interactive

if errorlevel 1 (
    call :log_error "Failed to register runner"
    exit /b 1
)

call :log_success "Runner registered"
exit /b 0

:setup_windows_service
call :log_info "Setting up Windows service..."

cd /d "!RUNNER_DIR!"

REM Create service
sc create gitea-runner ^
    binPath= "!RUNNER_DIR!\act_runner.exe daemon" ^
    start= auto ^
    DisplayName= "Gitea Actions Runner"

if errorlevel 1 (
    call :log_error "Failed to create service"
    exit /b 1
)

call :log_success "Windows service created"
exit /b 0

:start_service
call :log_info "Starting service..."

net start gitea-runner

if errorlevel 1 (
    call :log_error "Failed to start service"
    exit /b 1
)

call :log_success "Service started successfully"
exit /b 0

:verify_installation
call :log_info "Verifying installation..."

REM Check if runner configuration exists
if exist "!RUNNER_DIR!\.runner" (
    call :log_success "Runner configuration found"
) else (
    call :log_error "Runner configuration not found"
    exit /b 1
)

REM Check service status
sc query gitea-runner | find "RUNNING" >nul
if errorlevel 1 (
    call :log_error "Service is not running"
    exit /b 1
)

call :log_success "Service is running"
exit /b 0

REM ============================================================================
REM MAIN
REM ============================================================================

:main
call :log_info "Starting Gitea Actions Runner setup..."
call :log_info "Configuration:"
call :log_info "  Gitea URL: !GITEA_URL!"
call :log_info "  Runner Name: !RUNNER_NAME!"
call :log_info "  Runner Labels: !RUNNER_LABELS!"
call :log_info "  Installation Directory: !RUNNER_DIR!"

call :check_admin
if errorlevel 1 exit /b 1

call :check_requirements
if errorlevel 1 exit /b 1

call :install_dependencies
if errorlevel 1 exit /b 1

call :download_runner
if errorlevel 1 exit /b 1

call :register_runner
if errorlevel 1 exit /b 1

call :setup_windows_service
if errorlevel 1 exit /b 1

call :start_service
if errorlevel 1 exit /b 1

call :verify_installation
if errorlevel 1 exit /b 1

call :log_success "Gitea Actions Runner setup completed!"
call :log_info "Service status: Running"
call :log_info "View logs with: Get-EventLog -LogName System -Source 'Service Control Manager' | Where-Object {$_.EventID -eq 7036}"

endlocal
exit /b 0
