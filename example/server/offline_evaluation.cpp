#include"communication/server/offline_server.h"

std::unordered_map<std::string, std::string> args_keywords = {
  {"--option-file=", "Option file path"},
  {"--help", "Show usage"},
};

std::unordered_map<std::string, std::string> default_args = {
  {"--help", "false"},
};

void usage(){
  for( const auto &x : args_keywords ){
    std::cout << x.first << "    " << x.second << std::endl;
  }
  return;
}

int main(int argc, char *argv[]){
  const std::unordered_map<std::string, std::string> args = DAMCREM::util::parse_arguments(argc, argv, args_keywords, default_args);

  if( (args.count("--option-file=") == 0) || DAMCREM::util::convert_string<bool>(args.at("--help")) ){
    usage();
    return 0;
  }

  DAMCREM::communication::OfflineServer<> server(args.at("--option-file="));
  server.start();
  server.show_results(std::cout, true, true);
  
  return 0;
}

