#!/bin/bash
cd "${TARGET_BUILD_DIR}"

dist=("$FULL_PRODUCT_NAME")
if [ -d "$DWARF_DSYM_FILE_NAME" ]; then dist+=("$DWARF_DSYM_FILE_NAME"); fi
if [ -d "$SOURCE_ROOT/Tools" ]; then
    cp -a "$SOURCE_ROOT/Tools" "Tools"
    dist+=("Tools")
fi

archive="${PRODUCT_NAME}-${MODULE_VERSION}-$(echo $CONFIGURATION | tr /a-z/ /A-Z/).zip"
rm -rf *.zip
if [ "$CONFIGURATION" == "Release" ]; then
  strip -x -T "${EXECUTABLE_PATH}" &>/dev/null || strip -x "${EXECUTABLE_PATH}"
fi
zip -qry -FS "${archive}" "${dist[@]}"

rm -rf "Tools"

