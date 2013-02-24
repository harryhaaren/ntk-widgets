#!/bin/bash


g++ *.cxx `pkg-config --cflags --libs ntk` -o avtk_showcase && ./avtk_showcase
