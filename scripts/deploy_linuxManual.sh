#!/bin/bash

# Variables
APP_NAME="PokemonRandomizer"         # Name of your application
BUILD_DIR="../build/SVRandomizer"    # Directory containing the built executable
DEPLOY_DIR="./RandomizerRelease"                # Directory for the deployed application
QT_LIB_DIR="$HOME/Qt/6.8.0/gcc_64/lib"        # Path to Qt libraries
QT_PLUGIN_DIR="$HOME/Qt/6.8.0/gcc_64/plugins" # Path to Qt plugins

# Validate paths
if [ ! -f "$BUILD_DIR/$APP_NAME" ]; then
    echo "Error: Application executable not found: $BUILD_DIR/$APP_NAME"
    exit 1
fi

# Create deployment directory
echo "Creating deployment directory: $DEPLOY_DIR"
rm -rf "$DEPLOY_DIR"
mkdir -p "$DEPLOY_DIR/libs"
mkdir -p "$DEPLOY_DIR/plugins/platforms"
mkdir -p "$DEPLOY_DIR/plugins/imageformats"

# Function to copy a library and its dependencies
copy_library() {
    local lib="$1"
    if [ -f "$lib" ]; then
        echo "Copying library: $lib"
        cp -L "$lib" "$DEPLOY_DIR/libs/"  # Copy the library
        # Recursively copy dependencies
        ldd "$lib" | grep "=>" | awk '{print $3}' | while read -r dep; do
            if [ -f "$dep" ] && [ ! -f "$DEPLOY_DIR/libs/$(basename "$dep")" ]; then
                echo "  Copying dependency: $dep"
                cp -L "$dep" "$DEPLOY_DIR/libs/"
            fi
        done
    else
        echo "Warning: Library $lib not found!"
    fi
}

# Copy the application executable
echo "Copying application executable..."
cp "$BUILD_DIR/$APP_NAME" "$DEPLOY_DIR/"

# Copy required libraries for the application
echo "Copying required libraries for the application..."
ldd "$BUILD_DIR/$APP_NAME" | grep "=>" | awk '{print $3}' | while read -r lib; do
    copy_library "$lib"
done

# Explicitly copy Qt libraries and their dependencies
echo "Copying Qt core libraries and their dependencies..."
for lib in libQt6Core.so.6 libQt6Gui.so.6 libQt6Widgets.so.6 libQt6XcbQpa.so.6; do
    copy_library "$QT_LIB_DIR/$lib"
done

# Copy Qt plugins and their dependencies
echo "Copying Qt plugins and their dependencies..."
plugin="$QT_PLUGIN_DIR/platforms/libqxcb.so"
if [ -f "$plugin" ]; then
    echo "Copying plugin: $plugin"
    cp -L "$plugin" "$DEPLOY_DIR/plugins/platforms/"
    copy_library "$plugin"
else
    echo "Error: Qt platform plugin (libqxcb.so) not found in $QT_PLUGIN_DIR/platforms"
    exit 1
fi

# Copy Qt image format plugins
echo "Copying Qt image format plugins..."
cp $QT_PLUGIN_DIR/imageformats/libq*.so "$DEPLOY_DIR/plugins/imageformats/" || {
    echo "Error: Failed to copy image format plugins from $QT_PLUGIN_DIR/imageformats"
    exit 1
}

# Copy additional resources
echo "Copying additional resources..."
cp -r ../images "$DEPLOY_DIR/images"
cp -r ../thirdparty "$DEPLOY_DIR/thirdparty"
cp -r ../SV_FLATBUFFERS "$DEPLOY_DIR/SV_FLATBUFFERS"

# Update the rpath to use local libraries
echo "Updating rpath for the executable..."
patchelf --set-rpath '$ORIGIN/libs' "$DEPLOY_DIR/$APP_NAME"

# Update rpath for the platform plugin
echo "Updating rpath for the platform plugin..."
patchelf --set-rpath '$ORIGIN/../../libs' "$DEPLOY_DIR/plugins/platforms/libqxcb.so"

# Update rpath for Qt libraries
echo "Updating rpath for Qt libraries..."
for lib in "$DEPLOY_DIR/libs/"*.so*; do
    patchelf --set-rpath '$ORIGIN' "$lib"
done

# Create a wrapper script to set environment variables
echo "Creating wrapper script..."
cat > "$DEPLOY_DIR/Randomizer" <<EOL
#!/bin/bash
export QT_QPA_PLATFORM_PLUGIN_PATH="\$(dirname "\$0")/plugins/platforms"
export LD_LIBRARY_PATH="\$(dirname "\$0")/libs:\$LD_LIBRARY_PATH"
QT_DEBUG_PLUGINS=1 ./\$(dirname "\$0")/PokemonRandomizer
EOL
chmod +x "$DEPLOY_DIR/Randomizer"

# Deployment completed
echo "Deployment completed! Application bundle is located at: $DEPLOY_DIR"
