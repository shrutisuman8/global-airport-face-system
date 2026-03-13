#include "crow.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

struct User {
    int id;
    string name;
    string email;
    string password;
    vector<float> face_encoding;
};

struct Flight {
    int id;
    string flight_number;
    string destination;
};

struct Booking {
    int id;
    int user_id;
    int flight_id;
    string status;
};

vector<User> users;
vector<Flight> flights;
vector<Booking> bookings;

float cosine_similarity(vector<float>& v1, vector<float>& v2)
{
    float dot = 0;
    float norm1 = 0;
    float norm2 = 0;

    for(int i=0;i<v1.size();i++)
    {
        dot += v1[i]*v2[i];
        norm1 += v1[i]*v1[i];
        norm2 += v2[i]*v2[i];
    }

    return dot/(sqrt(norm1)*sqrt(norm2));
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app,"/auth/register").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);

        string name = body["name"].s();
        string email = body["email"].s();
        string password = body["password"].s();

        for(auto &u:users)
            if(u.email==email)
                return crow::response(400,"Email already registered");

        User u;
        u.id = users.size()+1;
        u.name=name;
        u.email=email;
        u.password=password;

        users.push_back(u);

        crow::json::wvalue res;
        res["message"]="Registration successful";
        res["user_id"]=u.id;
        res["name"]=u.name;

        return crow::response(res);
    });

    CROW_ROUTE(app,"/auth/login").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);

        string email = body["email"].s();
        string password = body["password"].s();

        for(auto &u:users)
        {
            if(u.email==email && u.password==password)
            {
                crow::json::wvalue res;
                res["message"]="Login successful";
                res["user_id"]=u.id;
                res["name"]=u.name;
                res["has_biometrics"]=!u.face_encoding.empty();
                return crow::response(res);
            }
        }

        return crow::response(401,"Invalid credentials");
    });

    CROW_ROUTE(app,"/bookings").methods("POST"_method)
    ([](const crow::request& req){

        auto body = crow::json::load(req.body);

        int user_id = body["user_id"].i();
        int flight_id = body["flight_id"].i();

        for(auto &b:bookings)
            if(b.user_id==user_id && b.flight_id==flight_id)
                return crow::response("Already booked");

        Booking b;
        b.id = bookings.size()+1;
        b.user_id=user_id;
        b.flight_id=flight_id;
        b.status="Pending";

        bookings.push_back(b);

        return crow::response("Flight booked");
    });

    CROW_ROUTE(app,"/flights").methods("GET"_method)
    ([](){

        crow::json::wvalue result;

        int i=0;
        for(auto &f:flights)
        {
            result[i]["id"]=f.id;
            result[i]["flight_number"]=f.flight_number;
            result[i]["destination"]=f.destination;
            i++;
        }

        return crow::response(result);
    });

    app.port(8000).multithreaded().run();
}