#!/bin/bash

# Update all Git submodules to the latest release
echo "Running Git submodule update --recursive --remote"
git submodule update --recursive --remote
