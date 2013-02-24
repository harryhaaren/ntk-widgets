#!/bin/bash


ntk-fluid -c avtk_test.fl &&

g++ *.cxx `pkg-config --cflags --libs ntk` -o avtk_showcase &&

./avtk_showcase
