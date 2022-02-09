#include"ldc/ldc.hpp"
#include<iostream>
#include<thread>
#include<mutex>

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

int main(int argc, char** args){
    if(argc < 2){
        std::cout << "This tool needs a max value for x and y\n";
        std::cout << "Usage: iotograph <y-max> [<window width> <window height>]\n";
        return 0;
    }
    float max_y = std::atof(args[1]);
    std::cout << "max: " << max_y << std::endl;
    int window_width = DEFAULT_WINDOW_WIDTH;
    int window_height = DEFAULT_WINDOW_HEIGHT;
    if(argc  >= 4){
        window_width = std::atoi(args[2]);
        window_height = std::atoi(args[3]);
    }
    
    std::vector<float> values; 
    std::vector<SDL_FPoint> points;
    ldc::RenderWindow rw("Terminal Graph Tool", window_width, window_height, false);
    
    bool iotRunning = true;
    std::thread ioThread([](std::vector<float>& values, ldc::RenderWindow& rw, float max_y, bool& iotRunning){
        while(iotRunning){
            std::string input;
            std::cin >> input;
            
            if(input.compare("end") == 0)
            {
                break;
            }
            values.push_back(std::atof(input.c_str()));
            std::cout << input << std::endl;
        }
    }, std::ref(values), std::ref(rw), max_y, std::ref(iotRunning));
    auto iotHandle = ioThread.native_handle();
    SDL_Event event;
    while(rw.isOpen()){
        while(rw.pollEvent(event)){
            if(event.type == SDL_QUIT){
                rw.close();
                iotRunning = false;
                pthread_cancel(iotHandle);
            }
        }
        rw.drawColor({.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE});
        rw.clear();
        if(values.size()){
            rw.drawColor({.r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE});
            int height = rw.getHeight();
            int width = rw.getWidth();

            for(int i = 0; i < values.size(); i++){
                SDL_FPoint p;
                if(i >= points.size()){
                    points.push_back(p);
                }
                points[i].x = (float)i * width / points.size();
                points[i].y = height * 0.5 - values[i] * height * 0.25 / max_y;
            }

            rw.drawLines(points);
        }
        rw.display();
    }
    ioThread.detach();
    for(auto a : points){
        std::cout << '\n' << a.x << ':' << a.y;
    }
    return 0;
}