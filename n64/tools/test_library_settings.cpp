#include "fe_options.h"
#include "fe_file_io.h"
#include "fe_nfs.h"
#include "fe_i18n.h"
#include "library_cache.h"
#include <filesystem>
#include <fstream>
#include <cassert>
int main(int argc,char**argv){assert(argc==2);std::string root=argv[1],error;
 std::filesystem::create_directories(root+"/cache");std::filesystem::create_directories(root+"/games");std::filesystem::create_directories(root+"/saves");
 std::ofstream(root+"/cache/library-0123456789abcdef.bin")<<"cache";std::ofstream(root+"/cache/other.bin")<<"keep";
 std::ofstream(root+"/games/Test.z64")<<"keep";std::ofstream(root+"/saves/Test.sav")<<"keep";
 std::ofstream(root+"/nfs.txt")<<"nfs://example/A\nnfs://example/B\n";
 fe::OptionsPaths paths;paths.gs_ini=root+"/gs.ini";paths.nfs_url=[&]{auto shares=fe::nfs::ConfiguredDirectories(root+"/nfs.txt");return shares.empty()?std::string{}:shares.front();};paths.save_nfs_url=[](const std::string&,std::string&){return true;};
 paths.reset_library=[&](std::string& err){return n64::ResetLibraryCache(root,err);};paths.clear_nfs_urls=[&](std::string&err){return fe::fileio::WriteAtomic(root+"/nfs.txt","# No NFS shares configured.\n",err);};
 fe::OptionsSheet sheet;sheet.Open(paths,nullptr);bool reset=false,clear=false;
 for(const auto& row:sheet.rows()){if(row.kind==fe::OptionsSheet::Kind::ResetLibrary){assert(fe::OptionsSheet::IsAction(row));assert(sheet.Activate(row,0));reset=true;}if(row.kind==fe::OptionsSheet::Kind::ClearNfsUrls){assert(fe::OptionsSheet::IsAction(row));assert(sheet.Activate(row,0));clear=true;}}
 assert(reset&&clear&&sheet.NfsUrl().empty());fe::OptionsSheet reopened;reopened.Open(paths,nullptr);assert(reopened.NfsUrl().empty());assert(fe::nfs::ConfiguredDirectories(root+"/nfs.txt").empty());
 assert(!std::filesystem::exists(root+"/cache/library-0123456789abcdef.bin"));assert(std::filesystem::exists(root+"/cache/rescan-library"));
 for(auto path:{"/cache/other.bin","/games/Test.z64","/saves/Test.sav"})assert(std::filesystem::exists(root+path));
 fe::SetLanguage(4,root+"/lang");assert(std::string(fe::LanguageCode())=="en");assert(std::string(fe::Tr(fe::Str::NoGames))=="No N64 games found");
 puts("English UI, cache-only reset, preservation of ROMs/saves and clearing all NFS shares passed (AI-assisted)");}
