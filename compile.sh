#!/bin/bash


ntk-fluid -c avtk_test.fl &&

g++ -fpermissive *.cxx `pkg-config --cflags --libs cairomm-1.0 ntk` -o avtk_showcase &&

./avtk_showcase
