#!/bin/sh

# Check the number of arguments passed
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

# Get the version argument
VERSION="$1"

# Build the image
docker buildx build --platform linux/arm64 -t "chillibits/spice:$VERSION" .

# Start the build
docker 