// (AI-assisted) Regional matching, URL escaping, fallback decoding/cache and stale 404 regression.
#include "cover_urls.h"
#include "fe_covers.h"
#include <cassert>
#include <fstream>
#include <filesystem>
#include <cstdio>
int main(int argc,char**argv){assert(argc==3);fe::GameInfo g;g.serial="N64-TEST";g.title="Super Mario 64";g.stem="Super Mario 64 (Europe) (En,Fr,De) (Rev 1)";g.file=g.stem+".zip";g.region="Europe";auto urls=n64::CoverUrls(g);assert(urls[0].find("Super%20Mario%2064%20%28E%29")!=std::string::npos);assert(urls.size()==3);g.region="USA";assert(n64::CoverUrls(g)[0].find("%28U%29")!=std::string::npos);g.region="Japan";assert(n64::CoverUrls(g)[0].find("%28J%29")!=std::string::npos);g.title="The Legend of Zelda - Ocarina of Time";g.region="Europe";assert(n64::CoverUrls(g)[0].find("Globaleliteee")!=std::string::npos);g.title="1080° Snowboarding";assert(n64::CoverUrls(g)[0].find("1080%20Snowboarding")!=std::string::npos);g.title="Pokémon Stadium";assert(n64::CoverUrls(g)[0].find("Pokemon%20Stadium")!=std::string::npos);g.title="Definitely absent original test";assert(n64::CoverUrls(g)[0].find("libretro-thumbnails")!=std::string::npos);
 std::ifstream f(argv[1],std::ios::binary);std::vector<uint8_t> image((std::istreambuf_iterator<char>(f)),{});assert(!image.empty());fe::CoverImage decoded;assert(fe::CoverService::Decode(image,1024,decoded));assert(decoded.width>0&&decoded.height>0);
 fe::CoverConfig cfg;cfg.cache_dir=argv[2];cfg.resolve_urls=[](const fe::GameInfo&){return std::vector<std::string>{"primary","fallback"};};std::filesystem::create_directories(cfg.cache_dir);std::ofstream(cfg.cache_dir+"/"+g.serial+".missing")<<"404\n";auto missing=fe::CoverService::MissingCovers({g},cfg);assert(missing.size()==1);int calls=0;auto download=[&](const std::string& url,std::vector<uint8_t>& data){++calls;if(url=="primary")return 404;data=image;return 200;};assert(fe::CoverService::Prefetch({g},{0},cfg,download,5)==1);assert(calls==2);assert(fe::CoverService::MissingCovers({g},cfg).empty());assert(std::filesystem::file_size(cfg.cache_dir+"/"+g.serial+".jpg")==image.size());assert(!std::filesystem::exists(cfg.cache_dir+"/"+g.serial+".n64covers-v2.missing"));
 printf("N64 cover regions, title aliases, source fallback, JPEG decode/cache and stale 404 recovery passed (AI-assisted)\n");}

