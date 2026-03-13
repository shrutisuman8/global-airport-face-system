#!/bin/bash

echo "Compiling gate camera..."
g++ gate_camera.cpp -o gate_camera -std=c++17 `pkg-config --cflags --libs opencv4`

echo "Compiling passenger enrollment..."
g++ enroll_passenger.cpp -o enroll_passenger -std=c++17 `pkg-config --cflags --libs opencv4`

echo "Compiling backend server..."
cd backend
g++ server.cpp -o server -std=c++17 -I ../Crow/include -I /opt/homebrew/include -lsqlite3

echo "Starting server..."
./server
