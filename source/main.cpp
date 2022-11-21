//
//  main.cpp
//  multicsvcmp
//
//  Created by Charles Kerr on 11/21/22.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>

#include "strutil.hpp"
#include "multi.hpp"

using namespace std::string_literals ;

int main(int argc, const char * argv[]) {
    try {
        auto uoppath = std::filesystem::path() ;
        auto mulpath = std::filesystem::path() ;
        auto missing = std::vector<std::uint32_t>() ;
        auto diff = std::vector<std::uint32_t>() ;
        if (argc != 3 ) {
            std::cerr <<"Usage:\n" ;
            std::cerr <<"\tmulticsvcmp source_csv_dir compare_csv_dir"<<std::endl;
            return 1 ;
        }
        else {
            mulpath  = std::filesystem::path(argv[1]);
            uoppath = std::filesystem::path(argv[2]);
            auto mulentries = loadCSV(mulpath);
            auto uopentries = loadCSV(uoppath) ;
            for (const auto &[id,entry]:mulentries){
                auto iter = uopentries.find(id) ;
                if (iter == uopentries.end()){
                    missing.push_back(id);
                }
                else {
                    if (entry != iter->second){
                        diff.push_back(id);
                    }
                }
            }
            if (!missing.empty()){
                std::cout <<"Items in " <<mulpath.string()<<" not found in " <<uoppath.string()<<":\n";
                for (auto const &id:missing){
                    std::cout <<"\t"<<id<<"\n";
                }
                std::cout <<std::endl;
            }
            if (!diff.empty()){
                std::cout <<"Items in " <<mulpath.string()<<" dffer from in " <<uoppath.string()<<":\n";
                for (auto const &id:diff){
                    std::cout <<"\t"<<id<<"\n";
                }
                std::cout <<std::endl;
            }
        }
    }
    catch(const std::exception &e){
        std::cerr <<e.what()<<std::endl;
        return 1 ;
    }
    return 0;
}
