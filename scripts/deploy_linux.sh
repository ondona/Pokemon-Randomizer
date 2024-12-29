#!/bin/bash
APP=$1
DEPLOY_DIR=$2
mkdir -p "$DEPLOY_DIR"
linuxdeployqt "$APP" -appimage
# Move the AppImage to the deployment directory
mv *.AppImage "$DEPLOY_DIR/"

echo "Deployment finished in $DEPLOY_DIR"
