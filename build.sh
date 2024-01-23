#!/usr/bin/env bash
if [ ! -d "bin" ]; then
	mkdir bin
fi
gcc src/main.c -D_ISO_DATE_=\"$(date +%F)\" -o bin/carnivore
