#include<iostream>
#include<cmath>

int main(){
    for(double i = 0; i < 6; i+=.01){
        std::cout << sin(i) << std::endl;
    }
    std::cout << "end" << std::endl;
}