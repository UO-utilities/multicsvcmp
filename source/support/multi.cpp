//Copyright © 2022 Charles Kerr. All rights reserved.

#include "multi.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "strutil.hpp"


using namespace std::string_literals;


//===========================================================================
auto csv_component_t::operator==(const csv_component_t &value) const ->bool {
    return (tileid == value.tileid) && (offsetx==value.offsetx) && (offsety == value.offsety) && (offsetz == value.offsetz) && (flag == value.flag);
}
//===========================================================================
auto csv_component_t::operator!=(const csv_component_t &value) const ->bool {
    return !this->operator==(value);
}

//===========================================================================
csv_component_t::csv_component_t(const std::string &entry) :csv_component_t() {
    auto comp = strutil::parse(entry,",") ;
    switch (comp.size()){
        default:
        case 5: {
            flag = strutil::ston<std::uint64_t>(comp.at(4));
            [[fallthrough]];
        }
        case 4: {
            offsetz = strutil::ston<std::int16_t>(comp.at(3));
            [[fallthrough]];
        }

        case 3: {
            offsety = strutil::ston<std::int16_t>(comp.at(2));
            [[fallthrough]];
        }
        case 2: {
            offsetx = strutil::ston<std::int16_t>(comp.at(1));
            [[fallthrough]];
        }
        case 1: {
            tileid = strutil::ston<std::uint16_t>(comp.at(0));
            [[fallthrough]];
        }
        case 0:{
            break;
        }
    }
}

//=================================================================================
//  csvfile ;
//=================================================================================
csvfile::csvfile(const std::filesystem::path &filepath) {
    auto input = std::ifstream(filepath.string());
    if (!input.is_open()){
        throw std::runtime_error("Unable to open: "s + filepath.string());
    }
    auto buffer = std::vector<char>(2049,0) ;
    while (input.good() && !input.eof()){
        input.getline(buffer.data(),2048);
        if (input.gcount()>0){
            std::string line = buffer.data() ;
            line = strutil::trim(line) ;
            auto [first,rest] = strutil::split(line,",") ;
            if (strutil::lower(first) != "tileid"){
                data.push_back(csv_component_t(line)) ;
            }
        }
    }
}
//=================================================================================
auto csvfile::operator==(const csvfile &value) const ->bool {
    auto rvalue = false ;
    if (this->data.size() == value.data.size()){
        rvalue = true ;
        for (size_t j=0 ; j< data.size();j++){
            if (data.at(j) != value.data.at(j)){
                rvalue = false ;
                break;
            }
        }
    }
    return rvalue ;
}
//=================================================================================
auto csvfile::operator!=(const csvfile &value) const ->bool {
    return !this->operator==(value) ;
}

//=================================================================================
auto loadCSV(const std::filesystem::path &path) ->std::map<std::uint32_t,csvfile>{
    auto rvalue = std::map<std::uint32_t,csvfile>() ;
    for (auto const &dir_entry : std::filesystem::recursive_directory_iterator(path)){
        if (strutil::lower(dir_entry.path().extension().string())==".csv"){
            // This could be one
            auto name =dir_entry.path().stem().string() ;
            auto loc = name.find_first_not_of("0") ;
            if (loc != std::string::npos){
                name = name.substr(loc) ;
            }
            else {
                name = "0";
            }
            try {
                auto id = static_cast<std::uint32_t>(std::stoul(name,nullptr,0)) ;
                rvalue.insert_or_assign(id, csvfile(dir_entry.path())) ;
            }
            catch(...) {
                std::cerr <<"Skipping non-id csv file: "<<dir_entry.path().string()<<std::endl;
            }
        }
    }
    return rvalue ;
}
