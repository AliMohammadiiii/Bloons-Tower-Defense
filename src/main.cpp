#include "game/Game.hpp"
#include <iostream>

int main(int argc,char* argv[]){
    if(argc<2){
        std::cerr<<"Usage: "<<argv[0]<<" <mapfile>\n";
        return 1;
    }
    try{
        Game g(argv[1]);
        g.run();
    }catch(const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<'\n';
        return 1;
    }
}
