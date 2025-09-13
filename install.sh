#!/bin/bash
# Web2App Manager Installation Script

set -e

echo "Installing Web2App Manager..."

# Check if GTK3 development libraries are installed
if ! pkg-config --exists gtk+-3.0; then
    echo "Installing GTK3 development libraries..."
    sudo apt update
    sudo apt install -y libgtk-3-dev build-essential cmake
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring build..."
cmake ..

# Build the application
echo "Building application..."
make

# Install binary to system
echo "Installing binary..."
sudo make install

# Create icon for the application
echo "Creating application icon..."
sudo mkdir -p /usr/share/pixmaps
sudo tee /usr/share/pixmaps/web2app-manager.svg > /dev/null << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<svg width="64" height="64" xmlns="http://www.w3.org/2000/svg">
  <rect width="64" height="64" fill="#0ea5e9" rx="8"/>
  <text x="32" y="45" font-family="Arial, sans-serif" font-size="28" fill="white" text-anchor="middle" font-weight="bold">W</text>
  <circle cx="50" cy="16" r="6" fill="white" opacity="0.8"/>
</svg>
EOF

# Install desktop file
echo "Installing desktop entry..."
sudo cp ../web2app-manager.desktop /usr/share/applications/
sudo sed -i 's|Icon=web2app-manager|Icon=/usr/share/pixmaps/web2app-manager.svg|' /usr/share/applications/web2app-manager.desktop

# Update desktop database
sudo update-desktop-database /usr/share/applications/

echo "Installation complete!"
echo "You can now find 'Web2App Manager' in your applications menu."
echo "To uninstall, run: sudo rm /usr/local/bin/web2app-manager /usr/share/applications/web2app-manager.desktop /usr/share/pixmaps/web2app-manager.svg"
