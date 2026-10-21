#!/bin/bash
# Usage: prune_bundle.sh path/to/App.app
# macdeployqt copies every SQL driver of Qt. These three need client libraries
# (ODBC, PostgreSQL, Mimer) that no Mac has by default and that nothing here
# uses; SQLite stays for the QML LocalStorage plugin.
set -e
rm -f "$1"/Contents/PlugIns/sqldrivers/libqsql{odbc,psql,mimer}.dylib
