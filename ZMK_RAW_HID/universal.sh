#!/bin/bash

# Copyright 2025 Marcos Ivan Chow Castro aka @mctechnology / @mctechnology17
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

### compile and universal START ###
# clone the repository only if not exist!
if [ ! -d qmk-hid-host.git ]; then git clone https://github.com/zzeneg/qmk-hid-host.git; fi
if [ ! -d qmk-hid-host.git ]; then cd qmk-hid-host; fi

#  config and download package/libs
# name = "qmk-hid-host"
APP_NAME="qmk-hid-host"
# build to Apple Silicon (M1/M2/M3)
rustup target add aarch64-apple-darwin
# build to Intel (x86_64)
rustup target add x86_64-apple-darwin

# Compile Release modus optimized for Intel
cargo build --release --target x86_64-apple-darwin
# Compile Release modus optimized for Apple Silicon
cargo build --release --target aarch64-apple-darwin

#  unification (universal binary)
# create a clean folder to save the final file
mkdir -p target/universal
# Handle Mach-O Universal Binaries.
# More information: <https://keith.github.io/xcode-man-pages/lipo.1.html>.
# - Create a universal file from two single-architecture files:
    # lipo path/to/binary_file.x86_64 path/to/binary_file.arm64e -create -output path/to/binary_file
lipo -create -output target/universal/$APP_NAME \
    target/x86_64-apple-darwin/release/$APP_NAME \
    target/aarch64-apple-darwin/release/$APP_NAME

#  VERIFICATION
# print to the screen what type of file we created (it should say "Mach-O universal binary")
file target/universal/$APP_NAME

# final message to know it's over
echo "✅ ready! your universal app is in: target/universal/$APP_NAME"

# run the universal binary you just created
# ./target/universal/qmk-hid-host
# return 1 herarchy back
cd -
### compile and universal END ###

# path where the original build is located
RUST_BUILD_PATH="qmk-hid-host/target/universal/qmk-hid-host"

# app configuration
APP_BASE_NAME="ZMK HID Runner"        # base name for the app
APP_BUNDLE="${APP_BASE_NAME}.app"     # full name of the app (with .app)
DMG_NAME="ZMK_HID_Runner_Installer.dmg"

# name that the binary will have inside our folder and the app
LOCAL_BINARY_NAME="zmk-hid-host"
BINARY_SOURCE="./$LOCAL_BINARY_NAME"

# other local resources
JSON_SOURCE="zmk-hid-host.json"
ICON_SOURCE="applet.icns"

# step 1: preparation and renaming
echo "🔍 looking for original binary..."

if [ -f "$RUST_BUILD_PATH" ]; then
    echo "✅ binary found in: $RUST_BUILD_PATH"
    echo "🚚 copying and renaming to '$LOCAL_BINARY_NAME' in the current folder..."
    cp "$RUST_BUILD_PATH" "$BINARY_SOURCE"
else
    echo "❌ critical error: original file not found."
    echo "   searched route: $RUST_BUILD_PATH"
    echo "   make sure you have compiled the rust project."
    exit 1
fi

# initial cleaning
echo "🧹 cleaning previous versions of the app..."
rm -rf "$APP_BUNDLE"
rm -f "$DMG_NAME"
rm -f temp_script.applescript

#  generar applescript
echo "📜 generating logic AppleScript..."
cat <<EOF > temp_script.applescript
on run
    -- get the path to the resources folder within the app
    set resourcesPath to (path to me as text) & "Contents:Resources:"

    -- define posix routes (using the already renamed name: zmk-hid-host)
    set zmkBinaryPath to POSIX path of (resourcesPath & "zmk-hid-host")
    set zmkJsonPath to POSIX path of (resourcesPath & "zmk-hid-host.json")

    -- give execute permissions to the binary (for security)
    try
        do shell script "chmod +x " & quoted form of zmkBinaryPath
    end try

    -- command to execute (nohup for background, muting output)
    set commandToRun to quoted form of zmkBinaryPath & " -c " & quoted form of zmkJsonPath

    try
        do shell script "nohup " & commandToRun & " > /dev/null 2>&1 &"
    on error errMsg
        display dialog "Error ZMK Runner: " & errMsg buttons {"OK"} default button "OK"
    end try
end run
EOF

# compile app
echo "🔨 compiling the application..."
osacompile -o "$APP_BUNDLE" temp_script.applescript

# inject resources
echo "📦 injecting resources into the app..."
mkdir -p "$APP_BUNDLE/Contents/Resources"

# we copy the binary (which we already brought and renamed in step 1)
cp "$BINARY_SOURCE" "$APP_BUNDLE/Contents/Resources/"
# Copiamos el JSON
if [ -f "$JSON_SOURCE" ]; then
    cp "$JSON_SOURCE" "$APP_BUNDLE/Contents/Resources/"
else
    echo "⚠️ warning: not found $JSON_SOURCE in the current folder."
fi

# configure plist (hide from dock)
echo "⚙️ configuring info.plist (agent mode)..."
# use -replace in case the key already exists, or -insert if not
plutil -replace LSUIElement -bool true "$APP_BUNDLE/Contents/Info.plist"

# icon (direct replacement strategy)
# TODO: test, sometimes the icon is not updated, I have to correct this!!
if [ -f "$ICON_SOURCE" ]; then
    echo "🎨 applying icon..."
    # cp "$ICON_SOURCE" "$APP_BUNDLE/Contents/Resources/applet.icns"
else
    echo "⚠️ was not found $ICON_SOURCE, the generic icon will be used."
fi

# company digital
echo "🔏 signing the application..."
codesign --force --deep --sign - "$APP_BUNDLE"

# we refresh the bundle so that finder notices the icon change
touch "$APP_BUNDLE"

#  crear dmg
if command -v create-dmg &> /dev/null; then
    echo "💿 creating dmg installer ($DMG_NAME)..."
    rm -f "$DMG_NAME"

    # pre-cleaning for safety
    if [ -d "/Volumes/ZMK HID Runner Installer" ]; then
         hdiutil detach "/Volumes/ZMK HID Runner Installer" -force > /dev/null 2>&1
    fi

    create-dmg \
      --volname "ZMK HID Runner Installer" \
      --window-pos 200 120 \
      --window-size 600 400 \
      --icon-size 100 \
      --icon "$APP_BUNDLE" 150 190 \
      --hide-extension "$APP_BUNDLE" \
      --app-drop-link 450 190 \
      "$DMG_NAME" \
      "$APP_BUNDLE"

    # dismount
    # if create-dmg left the volume mounted (resource busy error), we force it to exit.
    if [ -d "/Volumes/ZMK HID Runner Installer" ]; then
        echo "🧹 cleaning mounted volume..."
        sleep 2 # short wait for the system to release locks
        hdiutil detach "/Volumes/ZMK HID Runner Installer" -force > /dev/null 2>&1
    fi

    echo "✅ everything ready! created installer: $DMG_NAME"
else
    echo "⚠️ 'create-dmg' is not installed. only the .app."
fi

# final cleaning
rm temp_script.applescript
# optional: delete the copied local binary
# rm "$BINARY_SOURCE"

echo "🚀 process completed successfully."
