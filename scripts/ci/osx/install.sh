#!/bin/bash
set -ev

echo "install.sh"

brew update > /dev/null
brew install qt@5.10
#brew link --force qt@5.10
