#!/bin/bash
APP=$1
DEPLOY_DIR=$2
mkdir -p "$DEPLOY_DIR"
macdeployqt "$APP"
cp -R "$APP" "$DEPLOY_DIR/"