#!/usr/bin/env bash

gcc src/main.c -D_ISO_DATE_=\"$(date +%F)\" -o bin/carnivore
