#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Fake embedding generator
vector<float> extract_embedding(Mat &image)
{
    vector<float> embedding(512);

    for(int i=0;i<512;i++)
        embedding[i] = static_cast<float>(rand())/RAND_MAX;

    return embedding;
}

void save_embedding(const vector<float>& embedding, const string& path)
{
    ofstream file(path, ios::binary);

    int size = embedding.size();
    file.write((char*)&size,sizeof(int));
    file.write((char*)embedding.data(), size*sizeof(float));

    file.close();
}

int main()
{
    string passenger_id;

    cout<<"Enter Passenger ID: ";
    cin>>passenger_id;

    filesystem::create_directory("database");

    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout<<"Camera not detected\n";
        return -1;
    }

    CascadeClassifier face_detector;
    face_detector.load("/opt/homebrew/opt/opencv/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");

    Mat frame, gray;
    bool registered = false;

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Rect> faces;
        face_detector.detectMultiScale(gray, faces);

        for(auto face : faces)
        {
            rectangle(frame, face, Scalar(0,255,0),2);
        }

        if(!registered)
        {
            putText(frame,"Press SPACE to capture face",
            Point(30,30),FONT_HERSHEY_SIMPLEX,0.7,Scalar(255,255,255),2);
        }

        if(registered)
        {
            putText(frame,"Passenger Registered Successfully",
            Point(30,30),FONT_HERSHEY_SIMPLEX,0.7,Scalar(0,255,0),2);

            putText(frame,"Press Q to Exit",
            Point(30,60),FONT_HERSHEY_SIMPLEX,0.7,Scalar(0,255,0),2);
        }

        imshow("Airport Face Enrollment",frame);

        char key = waitKey(1);

        if(key == 32 && faces.size()>0 && !registered) // SPACE
        {
            Rect face = faces[0];
            Mat face_img = frame(face);

            vector<float> embedding = extract_embedding(face_img);

            string save_path = "database/" + passenger_id + ".bin";
            save_embedding(embedding,save_path);

            registered = true;
        }

        if(key=='q' || key=='Q')
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}