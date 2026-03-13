#include "crow.h"
#include <fstream>
#include <sstream>

int main()
{
    crow::SimpleApp app;

    // Serve homepage
    CROW_ROUTE(app, "/")([](){
        std::ifstream file("../frontend/index.html");

        std::stringstream buffer;
        buffer << file.rdbuf();

        return crow::response(buffer.str());
    });

    // Serve files
    CROW_ROUTE(app, "/<string>")([](std::string file){

        std::ifstream f("../frontend/" + file);

        if(!f)
            return crow::response(404);

        std::stringstream buffer;
        buffer << f.rdbuf();

        crow::response res(buffer.str());

        if(file.find(".css") != std::string::npos)
            res.set_header("Content-Type","text/css");

        if(file.find(".js") != std::string::npos)
            res.set_header("Content-Type","application/javascript");

        if(file.find(".html") != std::string::npos)
            res.set_header("Content-Type","text/html");

        return res;
    });

    // Register API
    CROW_ROUTE(app, "/auth/register").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req){

        crow::json::wvalue result;

        result["message"] = "Registration successful";
        result["user_id"] = 1;

        return crow::response(result);
    });

    // Login API
    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req){

        crow::json::wvalue result;

        result["message"] = "Login successful";
        result["user_id"] = 1;

        return crow::response(result);
    });

    // Flights
    CROW_ROUTE(app, "/flights")([](){

        crow::json::wvalue flights;

        flights[0]["id"] = 1;
        flights[0]["flight_number"] = "AC101";
        flights[0]["destination"] = "New York";

        flights[1]["id"] = 2;
        flights[1]["flight_number"] = "BA202";
        flights[1]["destination"] = "London";

        return crow::response(flights);
    });

    // Book flight
    CROW_ROUTE(app, "/bookings").methods(crow::HTTPMethod::Post)
    ([](){

        crow::json::wvalue result;

        result["status"] = "success";
        result["message"] = "Flight booked successfully";

        return crow::response(result);
    });

    // User flights
    CROW_ROUTE(app, "/users/<int>/flights")
    ([](int user_id){

        crow::json::wvalue flights;

        flights[0]["flight_number"] = "AC101";
        flights[0]["destination"] = "New York";
        flights[0]["status"] = "Pending";

        return crow::response(flights);
    });

    // Upload face
    CROW_ROUTE(app, "/users/<int>/biometrics").methods(crow::HTTPMethod::Post)
    ([](int user_id){

        crow::json::wvalue result;

        result["message"] = "Face uploaded";

        return crow::response(result);
    });

    // Scan passenger (FIXED POSITION)
    CROW_ROUTE(app, "/scan").methods(crow::HTTPMethod::Post)
    ([](){

        system("../gate_camera");

        return crow::response("Passenger verified");

    });

    // Start server
    app.port(18080).multithreaded().run();
}