#!/bin/bash
cd `dirname $0`

# ./premake4 clean

# todo: move this to premake, lua is better than bash
VERSION=`cat bundle_version.txt`
PLIST_TEMPLATE=Sample.plist.template
PLIST_TARGET_DIR=Project
mkdir -p $PLIST_TARGET_DIR
PLIST_TARGET=$PLIST_TARGET_DIR/Info.plist
sed -e s/GAME_VERSION_TEMPLATE/$VERSION/g $PLIST_TEMPLATE > $PLIST_TARGET || exit $?

./premake4 $@ --ios --gfxapi=gles2 --buildDependencyProjects xcode4 || exit $?

rm -rf ~/Library/Developer/Xcode/DerivedData/Nebulae-*

open ./Project/Nebulae.xcworkspace
