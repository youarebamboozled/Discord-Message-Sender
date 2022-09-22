#include <iostream>
#include <string>
#include <thread>
#include <cpr/cpr.h>
typedef char token[];
typedef std::string String;


char chVersion[8] = "1.0.0";
String rawURL = "https://discord.com/api/v9/channels/";
int nFrequency = 5;
int cTimes = 0;
long nChannel = 0;
String authToken;
String Message = "Hello World!";



[[noreturn]] void loop();
void times();
bool sendMessage();

int main(int argc, char *argv[]) {
    if (argc != 1)
        for (int i = 1; i < argc; i++) {
            if (String(argv[i]) == "-h" || String(argv[i]) == "--help"){
                std::cout << "Usage: " << argv[0] << R"( -a "Token" -c "Channel ID" [options])" << std::endl
                          << "Options:" << std::endl
                          << "  -h, --help\t\tShow this help message and exit" << std::endl
                          << "  -v, --version\t\tShow program's version number and exit" << std::endl
                          << "  -f, --frequency\tSet the frequency with which the message is sent (in seconds)"
                          << std::endl
                          << "  -m, --message\t\tSet the message to be send" << std::endl
                          << "  -t, --times\t\tSet the number of times the message is sent" << std::endl
                          << "  -c, --channel\t\tSet the channel id" << std::endl
                          << "  -a, --auth\t\tSet the auth token" << std::endl;
                return 0;
            }

            if (String(argv[i]) == "-v" || String(argv[i]) == "--version"){
                std::cout << "Current Version: " << chVersion << std::endl;
                return 0;
            }
            if (String(argv[i]) == "-m" || String(argv[i]) == "--message") {
                Message = argv[i + 1];
                std::cout << "Custom message set to: " << Message << std::endl;
            }
            if (String(argv[i]) == "-f" || String(argv[i]) == "--frequency")
                nFrequency = std::stoi(argv[i + 1]);
            if (String(argv[i]) == "-t" || String(argv[i]) == "--times")
                cTimes = std::stoi(argv[i + 1]);
            if (String(argv[i]) == "-c" || String(argv[i]) == "--channel")
                nChannel = std::stol(argv[i + 1]);
            if (String(argv[i]) == "-a" || String(argv[i]) == "--auth")
                authToken = argv[i + 1];
        }

    if (cTimes == 0) {
        std::cout << "Sending message every " << nFrequency << " seconds" << std::endl;
        loop();
    } else {
        std::cout << "Sending message " << cTimes << " times" << std::endl;
        times();
    }
}


[[noreturn]] void loop() {
    while (true) {
        if (sendMessage())
            std::cout << "Message sent!" << std::endl;
        else
            std::cout << "There was an error sending the message!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(nFrequency));
    }
}


void times() {
    if (sendMessage())
        std::cout << "Message sent!" << std::endl;
    else
        std::cout << "There was an error sending the message!" << std::endl;
    for (int i = 1; i < cTimes; i++) {
        if (sendMessage())
            std::cout << "Message sent!" << std::endl;
        else
            std::cout << "There was an error sending the message!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(nFrequency));
    }
}


bool sendMessage() {
    String URL = rawURL + std::to_string(nChannel) + "/messages";
    auto r = cpr::Post(cpr::Url{URL},
                       cpr::Payload{{"content", Message}},
                       cpr::Header{{"Authorization", authToken}});
    if (r.status_code == 200)
        return true;
    else if (r.status_code == 429) {
        std::cout << "Rate limited! Waiting 5 seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return sendMessage();
    }else if (r.status_code == 401) {
        std::cout << authToken << std::endl;
        std::cout << "Not Authorized! Did you entered the right auth Token?" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return sendMessage();
    }else
        std::cout << r.status_code << std::endl;
    std::cout << r.text << std::endl;
    return false;
}
