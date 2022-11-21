//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef multi_hpp
#define multi_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <ostream>
#include <map>
#include <utility>
#include <fstream>
#include <filesystem>
#include <vector>
//=================================================================================
//  csv_component_t ;
//=================================================================================
//=================================================================================
struct csv_component_t {
    std::uint16_t tileid ;
    std::int16_t offsetx ;
    std::int16_t offsety ;
    std::int16_t offsetz ;
    std::uint64_t flag ;
    csv_component_t():tileid(0xFFFF),offsetx(0),offsety(0),offsetz(0),flag(0){}
    csv_component_t(const std::string &entry) ;
    auto operator==(const csv_component_t &value) const ->bool ;
    auto operator!=(const csv_component_t &value) const ->bool ;
};

//=================================================================================
//  csvfile ;
//=================================================================================
struct csvfile {
    std::vector<csv_component_t> data ;
    csvfile() = default;
    csvfile(const std::filesystem::path &filepath) ;
    auto operator==(const csvfile &value) const ->bool ;
    auto operator!=(const csvfile &value) const ->bool ;

};

auto loadCSV(const std::filesystem::path &path) ->std::map<std::uint32_t,csvfile> ;
#endif /* multi_hpp */
