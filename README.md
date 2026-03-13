# global-airport-face-system
Global Airport Face System is a C++ and OpenCV based face recognition application designed to improve airport security. It detects faces in real time using a webcam and compares them with stored images to identify individuals quickly, demonstrating the use of computer vision for surveillance and identity verification..

Project Overview

The Face Recognition System is a computer vision application developed using C++ and OpenCV that detects and recognizes human faces in real time using a webcam.

The system captures video frames from a camera, detects faces in the frames, and compares them with stored images in a database to identify the person.

This project demonstrates the use of computer vision, image processing, and machine learning techniques for real-time facial recognition.

⸻

Objectives
	•	To build a real-time face recognition system using C++.
	•	To detect human faces from a live camera feed.
	•	To recognize faces by comparing them with stored images.
	•	To display the name or identity of the recognized person.

⸻

Technologies Used

Programming Language
	•	C++

Libraries
	•	OpenCV – for image processing and face detection
	•	STL (Standard Template Library) – for data structures and algorithms

Tools
	•	C++ Compiler (g++ / clang++)
	•	OpenCV Library
	•	Webcam
	•	Terminal / Command Line

⸻

System Architecture

The system follows the following processing pipeline:
	1.	Image Acquisition – Capture frames from webcam using OpenCV.
	2.	Face Detection – Detect faces using Haar Cascade classifier.
	3.	Face Preprocessing – Convert image to grayscale and normalize.
	4.	Face Recognition – Compare detected face with stored training images.
	5.	Output Display – Display recognized person’s name on screen.
  Project structure 
  Face-Recognition-System
│
├── database/
│   ├── person1.jpg
│   ├── person2.jpg
│
├── src/
│   ├── face_detection.cpp
│   ├── face_recognition.cpp
│
├── models/
│   └── haarcascade_frontalface_default.xml
│
├── build/
│
└── README.md
How the System Works

Step 1: Load Training Images

The system loads face images stored in the database folder.

Step 2: Initialize Face Detector

OpenCV’s Haar Cascade classifier is used to detect faces in images.

Step 3: Capture Video

The webcam captures real-time video frames using OpenCV’s VideoCapture.

Step 4: Detect Face

The Haar Cascade algorithm detects faces within each frame.

Step 5: Face Recognition

The detected face is compared with stored images using face recognition algorithms such as:
	•	Eigenfaces
	•	Fisherfaces
	•	Local Binary Pattern Histogram (LBPH)

Step 6: Display Result

If a match is found, the system displays the name of the recognized person.
If no match is found, it displays Unknown.
Installation

Step 1: Install OpenCV

On Mac/Linux:
brew install opencv

Step 2: Compile the Project
g++ face_recognition.cpp -o face_recognition `pkg-config --cflags --libs opencv4`

Step 3: Run the Program
./face_recognition

The webcam will start and the system will begin detecting and recognizing faces.

⸻

Requirements
	•	C++ compiler (g++ or clang++)
	•	OpenCV library
	•	Webcam
	•	Haar Cascade XML file

⸻

Features
	•	Real-time face detection
	•	Face recognition using stored images
	•	Fast processing using OpenCV
	•	Simple and efficient implementation

⸻

Applications
	•	Smart attendance systems
	•	Security and surveillance systems
	•	Biometric authentication
	•	Access control systems

⸻

Limitations
	•	Accuracy depends on lighting conditions.
	•	Recognition performance decreases with poor image quality.
	•	Requires clear facial images for better recognition.

⸻

Future Improvements
	•	Implement deep learning-based face recognition.
	•	Add a graphical user interface.
	•	Integrate with a database system.
	•	Deploy as a web or mobile application.

⸻

Conclusion

This project demonstrates the implementation of a real-time face recognition system using C++ and OpenCV. The system successfully detects and identifies human faces from a webcam feed and shows how computer vision can be applied in security and authentication applications.

⸻
