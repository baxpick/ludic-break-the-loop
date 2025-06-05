#!/usr/bin/env bash

curl -L -o build.sh https://raw.githubusercontent.com/baxpick/cpctelera_example/main/build_cpctelera_project_using_container.sh

rm -rf OUTPUT >/dev/null 2>&1§
mkdir -p OUTPUT/CPC >/dev/null 2>&1
mkdir -p OUTPUT/ENTERPRISE >/dev/null 2>&1

./build.sh \
    --folder-src . \
    --folder-output ./OUTPUT/ENTERPRISE \
    --platform enterprise \
    --buildcfg-z80ccflags "-DENTERPRISE"

./build.sh \
    --folder-src . \
    --folder-output ./OUTPUT/CPC \
    --platform cpc
