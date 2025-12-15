# ZMK RAW HID Implementation (15-12-2025)

This document describes the RAW HID implementation for the nice!OLED, nice!view and nice!vista508.

## 0. Quick Start
1. Compile your `.uf2` and flash your keyboard including the `zmk-nice-oled` shield as indicated in the instructions in the README.md
   - don't forget to activate the flag `CONFIG_NICE_OLED_WIDGET_RAW_HID=y`
2. Download and install the `zmk-hid-host` program to your computer and configure it to always start from login
   - `zmk-hid-host` does not spend much `0%` cpu resources!
   - modify the `json` file if necessary!
   - `zmk-hid-host` is compatible for `intel` and apple `m` series processors, it is a universal binary!
3. Enjoy your keyboard!!!
   - remember to customize to your liking.
   - TIP: you can activate seeing the weather and seeing your spotify song on macos!

## 1. Introduction to RAW HID

RAW HID provides a flexible way to send and receive custom data between the keyboard and the host computer. This allows for advanced features that are not covered by standard HID profiles.

To enable RAW HID, you need to set the following options in your `Kconfig.defconfig`:

```
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
```

This will enable the RAW HID interface and the corresponding widgets.

## 2. Available RAW HID Features

The following features are implemented using RAW HID:

### 2.1. Layout Switching

This feature allows you to display the current keyboard layout on the OLED screen. The layout can be sent from the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_LIST="DE,US,LA,AB"
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_LAYOUT_CUSTOM_Y=86
```

The `LAYOUT_LIST` is a comma-separated list of layout names that will be displayed on the screen based on the index received from the host.

### 2.2. Weather Widget (macOS Only)

This widget displays the current temperature received from the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_WEATHER_CUSTOM_Y=62
```

The host application needs to send the temperature value to the keyboard. **This feature is currently optimized for macOS.**

### 2.3. Spotify Media Player Widget (macOS Only)

This widget displays the current song playing on Spotify on a macOS computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_SPOTIFY_MACOS=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_MEDIA_PLAYER_CUSTOM_Y=110
```

A host application is required to read the song information from Spotify and send it to the keyboard. **This feature is exclusive to macOS.**

### 2.4. System Time Widget

This widget displays the current system time synchronized with the host computer.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_TIME_CUSTOM_Y=74
```

**This feature is compatible with Windows, Linux, and macOS.**

### 2.5. System Volume Widget

This widget displays the current system audio volume.

**Kconfig Options:**

```
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME=y
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_X=0
CONFIG_NICE_OLED_WIDGET_RAW_HID_VOLUME_CUSTOM_Y=98
```

**This feature is compatible with Windows, Linux, and macOS.**

## 3. Host-side application

The companion host application, `qmk-hid-host`, provides the necessary data bridge for universal Raw HID features (System Time, Volume, Layout) across Linux, Windows, and macOS.

*   **Technical Contributions (macOS Enhancements)**:
    *   **macOS Temperature Support**: Implemented platform-specific temperature monitoring for macOS within the Rust codebase.
    *   **Spotify Integration**: Engineered a native AppleScript/Rust bridge to fetch real-time track metadata from Spotify for display on the peripheral (macOS only).
    *   **Cross-Architecture Support**: Developed a universal macOS application wrapper, bundling native binaries for both Intel (`x86_64`) and Apple Silicon (`arm64`) architectures, ensuring seamless deployment across the Apple ecosystem.

You can compile the application from the source by following these steps:

1.  Clone the original repository:
    ```bash
    git clone https://github.com/zzeneg/qmk-hid-host.git
    ```
2.  Navigate to the project directory:
    ```bash
    cd qmk-hid-host
    ```
3.  Compile the project in release mode:
    ```bash
    cargo build --release
    ```
4.  The binary will be available in `target/release/`.

Once compiled, you can run the application. It will automatically detect your keyboard and start sending the required data.

## 4. Creating a `.dmg` for the macOS application

Once you have your host-side application, you can create a `.dmg` file for easy distribution and installation.

You can use the `create-dmg` tool, which can be installed via Homebrew:

```bash
brew install create-dmg
```

Then, you can create the `.dmg` file with the following command:

```bash
create-dmg \
  --volname "ZMK RAW HID Host" \
  --background "path/to/your/background.png" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon-size 100 \
  --icon "YourApp.app" 170 200 \
  --hide-extension "YourApp.app" \
  --app-drop-link 430 200 \
  "ZMK_RAW_HID_Host.dmg" \
  "source_folder/"
```

Replace `"YourApp.app"` and `"source_folder/"` with your actual application and source folder.

## 5. Running the application on startup

To make the host application run automatically every time you start your Mac, you can add it to the "Login Items".

1.  Open **System Settings**.
2.  Go to **General** > **Login Items**.
3.  Click the **+** button.
4.  Select your application from the `/Applications` folder.
5.  Click **Open**.

Now, the application will start automatically when you log in, ensuring that the RAW HID features of your keyboard are always available.

## 6. Updating the host-side application

If you need to update the host-side application with a new binary or configuration file, you can do so by replacing the files inside the application bundle.

1.  Go to your `Applications` folder.
2.  Right-click on `ZMK HID Runner.app` and select "Show Package Contents".
3.  Navigate to `Contents/Resources/`.
4.  You will see the following structure:
    ```
    .
    ├── applet.icns
    ├── applet.rsrc
    ├── description.rtfd
    │   └── TXT.rtf
    ├── Scripts
    │   └── main.scpt
    ├── zmk-hid-host
    └── zmk-hid-host.json
    ```
5.  You can replace the `zmk-hid-host` binary and the `zmk-hid-host.json` configuration file with your new versions. Make sure the new files have the same names.

> [!NOTE]
> You can also use the `cp` command to replace the files. For example, if your new files are in the current directory, you can use the following commands:
> ```bash
> cp zmk-hid-host "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host"
> cp zmk-hid-host.json "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host.json"
> ```
> If for some reason the application does not work after replacing the files, check the file permissions. The binary should have execution permissions. You can set them with `chmod +x`.
> ```bash
> chmod +x "/Applications/ZMK HID Runner.app/Contents/Resources/zmk-hid-host"
> ```

## Author and Contributions

The original `qmk-hid-host` project was created by **zzeneg**, and you can find the repository here: [https://github.com/zzeneg/qmk-hid-host](https://github.com/zzeneg/qmk-hid-host).

The **Spotify** and **Weather** functionalities described in this document were added in this pull request: [https://github.com/zzeneg/qmk-hid-host/pull/24/files](https://github.com/zzeneg/qmk-hid-host/pull/24/files).

The RAW HID functionality in this project is a custom implementation, ported from the great work of **@zzeneg** in the following repositories:
- [zmk-raw-hid](https://github.com/zzeneg/zmk-raw-hid)
- [zmk-nice-view-hid](https://github.com/zzeneg/zmk-nice-view-hid)

This implementation is 100% independent and is activated only with the following flags, without needing any other files:
```
CONFIG_NICE_OLED_WIDGET_RAW_HID=y
```
