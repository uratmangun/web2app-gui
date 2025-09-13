# GitButler Installation Guide with Distrobox using AppImage

This guide shows how to install GitButler using distrobox with a Fedora 40 container to resolve GLIBC compatibility issues.

## Prerequisites

- Linux system with container support (Podman/Docker)
- Internet connection for downloading packages

## Step 1: Install Distrobox

```bash
curl -s https://raw.githubusercontent.com/89luca89/distrobox/main/install | sudo sh
```

## Step 2: Create Fedora 40 Container

```bash
distrobox create --name gitbutler-env --image fedora:40
```

## Step 3: Enter Container and Install Dependencies

```bash
distrobox enter gitbutler-env -- bash -c "sudo dnf update -y && sudo dnf install -y wget fuse fuse-libs fribidi gtk3 webkit2gtk4.1"
```

## Step 4: Download GitButler AppImage

```bash
distrobox enter gitbutler-env -- bash -c "cd /tmp && wget https://app.gitbutler.com/downloads/release/linux/x86_64/gz -O GitButler.AppImage.gz"
```

## Step 5: Extract and Install AppImage

```bash
distrobox enter gitbutler-env -- bash -c "cd /tmp && tar -xzf GitButler.AppImage.gz"
distrobox enter gitbutler-env -- bash -c "cd /tmp && mkdir -p ~/.local/bin && cp GitButler_0.16.1_amd64.AppImage ~/.local/bin/gitbutler && chmod +x ~/.local/bin/gitbutler"
```

## Step 6: Install Git and Setup CLI Tools

Install git in the container:
```bash
distrobox enter gitbutler-env -- bash -c "sudo dnf install -y git"
```

Create symlink for GitButler CLI tool ('but' command):
```bash
# First, run GitButler to mount the AppImage, then create symlink
distrobox enter gitbutler-env -- bash -c "~/.local/bin/gitbutler &"
# Wait a moment for AppImage to mount, then create symlink
distrobox enter gitbutler-env -- bash -c "sudo ln -sf '/tmp/.mount_gitbut*/usr/bin/but' /usr/local/bin/but"
```

## Step 7: Configure SSH and Git Credentials

Fix SSH permissions in the container (distrobox shares your home directory, but permissions may need adjustment):
```bash
distrobox enter gitbutler-env -- bash -c "chmod 700 ~/.ssh && chmod 600 ~/.ssh/config ~/.ssh/id_* && chmod 644 ~/.ssh/*.pub"
```

Set up git configuration to match your host system:
```bash
# Replace with your actual git username and email
distrobox enter gitbutler-env -- bash -c "git config --global user.name 'your-username'"
distrobox enter gitbutler-env -- bash -c "git config --global user.email 'your-email@example.com'"
```

Test SSH connectivity to GitHub:
```bash
distrobox enter gitbutler-env -- bash -c "ssh -T git@github.com"
```
*Expected output: "Hi username! You've successfully authenticated, but GitHub does not provide shell access."*

## Step 8: Test GitButler Installation

```bash
distrobox enter gitbutler-env -- bash -c "~/.local/bin/gitbutler --version"
distrobox enter gitbutler-env -- bash -c "but --help"
distrobox enter gitbutler-env -- bash -c "git --version"
```

## Step 9: Create Host System Launcher Script

Create `/home/uratmangun/.local/bin/gitbutler-launcher`:

```bash
#!/bin/bash
# GitButler Launcher Script
# Runs GitButler from distrobox container

# Enable X11 forwarding for GUI applications
export DISPLAY=${DISPLAY:-:0}

# Launch GitButler in the distrobox container
exec distrobox enter gitbutler-env -- ~/.local/bin/gitbutler "$@"
```

Make it executable:
```bash
chmod +x ~/.local/bin/gitbutler-launcher
```

## Step 10: Create Desktop Entry

Create `/home/uratmangun/.local/share/applications/gitbutler.desktop`:

```ini
[Desktop Entry]
Name=GitButler
Comment=Git branch management tool
Exec=/home/uratmangun/.local/bin/gitbutler-launcher
Icon=/home/uratmangun/.local/share/icons/gitbutler-tauri.png
Terminal=false
Type=Application
Categories=Development;RevisionControl;
StartupNotify=true
StartupWMClass=GitButler
```

## Step 11: Extract and Install GitButler Icon

```bash
# Extract AppImage contents
distrobox enter gitbutler-env -- bash -c "cd /tmp && ~/.local/bin/gitbutler --appimage-extract"

# Copy icon to host system
distrobox enter gitbutler-env -- bash -c "cp /tmp/squashfs-root/usr/share/icons/hicolor/128x128/apps/gitbutler-tauri.png /home/uratmangun/"
mkdir -p ~/.local/share/icons
cp ~/gitbutler-tauri.png ~/.local/share/icons/

# Update desktop database
update-desktop-database ~/.local/share/applications/
```

## Usage

After installation, you can use GitButler in several ways:

1. **From Applications Menu**: Search for "GitButler" and click to launch
2. **Command Line**: Run `gitbutler-launcher`
3. **Direct Container Access**: `distrobox enter gitbutler-env -- ~/.local/bin/gitbutler`
4. **GitButler CLI**: `distrobox enter gitbutler-env -- but <command>`
5. **Git Commands**: `distrobox enter gitbutler-env -- git <command>`

## Why This Method Works

- **GLIBC Compatibility**: Fedora 40 provides GLIBC 2.39, meeting GitButler's requirements
- **Minimal Footprint**: Uses the smallest suitable Linux distribution
- **AppImage Benefits**: Self-contained, no dependency conflicts
- **Desktop Integration**: Full GUI support with proper icon and menu entry
- **Isolation**: Runs in container, doesn't affect host system

## Troubleshooting

- If GUI doesn't appear, ensure X11 forwarding is working: `echo $DISPLAY`
- For Wayland users, you may need to install `xwayland`
- If FUSE errors occur, ensure FUSE is installed in the container

## Container Management

- **List containers**: `distrobox list`
- **Stop container**: `distrobox stop gitbutler-env`
- **Remove container**: `distrobox rm gitbutler-env`
- **Container size**: ~2GB (Fedora 40 + dependencies + GitButler)

This method provides a clean, isolated GitButler installation that works around GLIBC version incompatibilities on older host systems.
