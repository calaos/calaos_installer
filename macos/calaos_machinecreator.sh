#!/bin/sh
script="$0"
basename="$(dirname $script)"
osascript $basename/main.scpt
