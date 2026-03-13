#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

float cosine_similarity(const vector<float>& v1, const vector<float>& v2)
{
    float dot = 0, norm1 = 0, norm2 = 0;

    for(int i=0;i<v1.size();i++)
    {
        dot += v1[i]*v2[i];
        norm1 += v1[i]*v1[i];
        norm2 += v2[i]*v2[i];
    }

    return dot / (sqrt(norm1) * sqrt(norm2));
}

vector<float> load_embedding(const string& path)
{
    ifstream file(path, ios::binary);

    int size;
    file.read((char*)&size,sizeof(int));

    vector<float> embedding(size);
    file.read((char*)embedding.data(), size*sizeof(float));

    return embedding;
}

map<string, vector<float>> load_passenger_database(string db_path="database")
{
    map<string, vector<float>> db;

    for(auto& entry : fs::directory_iterator(db_path))
    {
        if(entry.path().extension()==".bin")
        {
            string passenger_id = entry.path().stem().string();
            db[passenger_id] = load_embedding(entry.path().string());
        }
    }

    return db;
}

vector<float> extract_embedding(Mat& frame)
{
    vector<float> embedding(512);

    for(int i=0;i<512;i++)
        embedding[i] = static_cast<float>(rand())/RAND_MAX;

    return embedding;
}

void start_gate_camera()
{
    cout<<"Loading passenger database...\n";

    auto passenger_db = load_passenger_database();

    cout<<"Loaded "<<passenger_db.size()<<" passengers\n";

    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout<<"Camera failed\n";
        return;
    }

    const float MATCH_THRESHOLD = 0.65;

    Mat frame;

    string message = "";
    Scalar color = Scalar(255,255,255);

    while(true)
    {
        cap >> frame;

        if(frame.empty())
            break;

        putText(frame,"Press S to scan | Q to quit",
                Point(20,40),
                FONT_HERSHEY_SIMPLEX,
                0.7,
                Scalar(255,255,255),
                2);

        if(message!="")
        {
            putText(frame,message,
                    Point(20,80),
                    FONT_HERSHEY_SIMPLEX,
                    0.8,
                    color,
                    2);
        }

        imshow("Airport Gate Scanner",frame);

        char key = waitKey(1);

        if(key=='s' || key=='S')
        {
            vector<float> live_embedding = extract_embedding(frame);

            string best_match="";
            float highest_score=0;

            for(auto& p : passenger_db)
            {
                float similarity = cosine_similarity(live_embedding,p.second);

                if(similarity>highest_score)
                {
                    highest_score=similarity;
                    best_match=p.first;
                }
            }

            if(highest_score > MATCH_THRESHOLD)
            {
                message = "Boarding Approved: " + best_match;
                color = Scalar(0,255,0);
            }
            else
            {
                message = "Access Denied";
                color = Scalar(0,0,255);
            }
        }

        if(key=='q' || key=='Q')
            break;
    }

    cap.release();
    destroyAllWindows();
}

int main()
{
    start_gate_camera();
}