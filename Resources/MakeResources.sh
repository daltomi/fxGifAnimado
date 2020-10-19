#!/bin/bash

rm Resources.h
reswrap -k -o Resources.h *.png
reswrap -k -oa Resources.h *.xpm
reswrap -k -oa Resources.h *.gif

