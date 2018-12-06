#!/bin/env bash

rm -fr docpath
mkdir -p docpath/fr
mkdir -p docpath/en

export LC_ALL=C
export LANG=C
export LANGUAGE=C
calaos_server --help
calaos_server --gendoc docpath/en

export LC_ALL=fr_FR.UTF-8
export LANGUAGE=fr_FR.UTF-8
export LANG=fr_FR.UTF-8
calaos_server --help
calaos_server --gendoc docpath/fr

