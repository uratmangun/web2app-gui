# Web2App Manager

A native Linux desktop application for creating and managing web application shortcuts with full dock and favorites integration.

![Web2App Manager Screenshot](https://img.shields.io/badge/Platform-Linux-blue) ![Build Status](https://img.shields.io/badge/Build-Passing-green) ![License](https://img.shields.io/badge/License-MIT-orange)

## Overview

Web2App Manager is a GTK-based desktop application that allows you to easily convert any website into a standalone desktop application on Linux. Created web apps:

- 📱 Run in their own browser window without tabs or toolbars
- 🔗 Have their own isolated browser profile and data directory
- 🎨 Display custom icons (downloaded or auto-generated)
- 📌 Can be added to your dock and favorites
- 🚀 Launch from your applications menu like native apps

## Features

- **Create Web Apps**: Convert any URL into a desktop application
- **Icon Management**: Download icons from URLs or auto-generate colored letter icons
- **Favorites Integration**: One-click addition to Pop!_OS dock and favorites
- **App Management**: List, launch, and remove installed web applications
- **Browser Support**: Compatible with Brave, Chrome, and Chromium browsers
- **Native Integration**: Full GNOME/Pop!_OS desktop environment integration

## Prerequisites

### System Requirements

- **Operating System**: Linux (Pop!_OS, Ubuntu, Debian, or other GNOME-based distributions)
- **Desktop Environment**: GNOME 3.x or later
- **Browser**: One of the following:
  - Brave Browser (`brave-browser`)
  - Google Chrome (`google-chrome`)
  - Chromium (`chromium-browser` or `chromium`)

### Build Dependencies

```bash
# Ubuntu/Pop!_OS/Debian
sudo apt update
sudo apt install -y build-essential cmake pkg-config libgtk-3-dev

# Fedora
sudo dnf install -y gcc-c++ cmake pkgconfig gtk3-devel

# Arch Linux
sudo pacman -S base-devel cmake pkgconfig gtk3
```

### Runtime Dependencies

```bash
# For icon conversion (optional but recommended)
sudo apt install -y librsvg2-bin imagemagick

# Or for Fedora
sudo dnf install -y librsvg2-tools ImageMagick

# Or for Arch
sudo pacman -S librsvg imagemagick
```

## Installation

### Method 1: Quick Install (Recommended)

1. **Clone or download** this repository:
   ```bash
   git clone <repository-url>
   cd cpp-gui-app
   ```

2. **Run the install script**:
   ```bash
   chmod +x install.sh
   ./install.sh
   ```

   This will automatically:
   - Install required dependencies
   - Build the application
   - Install the binary to `/usr/local/bin/web2app-manager`
   - Create a desktop entry for the applications menu
   - Set up the application icon

### Method 2: Manual Build

1. **Create build directory**:
   ```bash
   mkdir build && cd build
   ```

2. **Configure with CMake**:
   ```bash
   cmake ..
   ```

3. **Build the application**:
   ```bash
   make
   ```

4. **Install system-wide** (optional):
   ```bash
   sudo make install
   ```

5. **Create desktop entry** (if installing system-wide):
   ```bash
   sudo cp ../web2app-manager.desktop /usr/share/applications/
   sudo update-desktop-database /usr/share/applications/
   ```

### Method 3: Local Installation

To install only for the current user without sudo:

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
make
make install

# Create user desktop entry
mkdir -p ~/.local/share/applications
cp ../web2app-manager.desktop ~/.local/share/applications/
update-desktop-database ~/.local/share/applications/
```

## Usage

### Launching the Application

- **From Applications Menu**: Search for "Web2App Manager"
- **From Terminal**: Run `web2app-manager`
- **From File Manager**: Double-click the built executable

### Creating Web Apps

1. **Enter App Name**: Choose a descriptive name for your web app
2. **Enter URL**: The website URL (http/https prefix optional)
3. **Enter Icon URL** (optional): Direct link to an icon image
   - If omitted, a colored icon with the first letter will be generated
4. **Click "Create Web App"**

### Managing Web Apps

- **View Apps**: All created web apps appear in the list
- **Add to Favorites**: Select an app and click "Add to Favorites" to add it to your dock
- **Remove Apps**: Select an app and click "Remove Selected App" to uninstall
- **Refresh List**: Click "Refresh List" to update the display

### Adding to Dock/Favorites

1. **Using the GUI**: Select an app and click "Add to Favorites"
2. **Manual Method**: 
   - Launch the web app
   - Right-click its icon in the dock → "Add to Favorites"
3. **From Applications Menu**: 
   - Find the app in your launcher
   - Right-click → "Add to Favorites"

## File Locations

- **Application Binary**: `/usr/local/bin/web2app-manager`
- **Desktop Files**: `~/.local/share/applications/[appname].desktop`
- **Icons**: `~/.local/share/icons/webapps/[appname].png`
- **App Data**: `~/.config/webapps/[appname]/`

## Troubleshooting

### Browser Not Found Error

**Problem**: "No Chrome/Chromium browser found!"

**Solution**: Install a supported browser:
```bash
# Install Brave Browser
sudo apt install brave-browser

# Or install Chromium
sudo apt install chromium-browser

# Or install Google Chrome
wget -q -O - https://dl.google.com/linux/linux_signing_key.pub | sudo apt-key add -
echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" | sudo tee /etc/apt/sources.list.d/google-chrome.list
sudo apt update
sudo apt install google-chrome-stable
```

### App Not Appearing in Favorites

**Problem**: Created app doesn't show in dock favorites

**Solutions**:
1. **Restart GNOME Shell**: Press `Alt+F2`, type `r`, press Enter
2. **Update desktop database**: 
   ```bash
   update-desktop-database ~/.local/share/applications/
   ```
3. **Check .desktop file**: Ensure it's properly formatted in `~/.local/share/applications/`

### Icon Issues

**Problem**: Icons not displaying or converting

**Solution**: Install image conversion tools:
```bash
sudo apt install librsvg2-bin imagemagick
```

### Permission Errors

**Problem**: Permission denied when installing

**Solution**:
- For system-wide install: Use `sudo` with install commands
- For user install: Use the local installation method above

## Development

### Building from Source

```bash
# Install development dependencies
sudo apt install -y build-essential cmake pkg-config libgtk-3-dev

# Clone and build
git clone <repository-url>
cd cpp-gui-app
mkdir build && cd build
cmake ..
make
```

### Project Structure

```
cpp-gui-app/
├── CMakeLists.txt          # Build configuration
├── src/
│   └── main.cpp           # Main application source
├── web2app-manager.desktop # Desktop entry file
├── install.sh             # Installation script
└── README.md              # This file
```

## Uninstallation

### Complete Removal

```bash
# Remove application binary
sudo rm /usr/local/bin/web2app-manager

# Remove desktop entry
sudo rm /usr/share/applications/web2app-manager.desktop

# Remove application icon
sudo rm /usr/share/pixmaps/web2app-manager.svg

# Update desktop database
sudo update-desktop-database /usr/share/applications/

# Optional: Remove created web apps
rm -rf ~/.local/share/applications/*webapp*.desktop
rm -rf ~/.local/share/icons/webapps/
rm -rf ~/.config/webapps/
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on Pop!_OS/Ubuntu
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by [Omakub's web2app functionality](https://github.com/basecamp/omakub)
- Built with GTK3 for native Linux desktop integration
- Thanks to the GNOME and Pop!_OS communities

## Support

For issues, questions, or contributions:
- Open an issue on GitHub
- Check the troubleshooting section above
- Ensure your system meets the prerequisites

---

**Enjoy using Web2App Manager!** 🚀
