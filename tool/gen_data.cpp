#include<iostream>

#include"executor.h"
#include"option.h"

// keyword, description
const std::unordered_map<std::string, std::string> args_keywords = {
  {"--option=", "option file path"},
  {"--app-id=", "App ID"},
  {"--client-id=", "Client ID"},
  {"--pt", "Target is plaintext."},
  {"--ct", "Target is ciphertext."},
  {"--index=", "Index of target plaintext/ciphertext"},
  {"--scalar-value=", "Value if the values of all the slots is the same."},
  {"--level=", "Level of the plaintext/ciphertext"},
  {"--output-dir=", "output directory. (const|input)"},
};

void usage(){
  for( const auto &x : args_keywords ){
    std::cout << x.first << ": " << x.second << std::endl;
  }
}

int main(int argc, char *argv[]){
  if( argc == 1 ){
    usage();
    return 0;
  }

  std::unordered_map<std::string, std::string> default_args;
  default_args["--pt"] = "false";
  default_args["--ct"] = "false";
  
  std::unordered_map<std::string, std::string> args = DAMCREM::util::parse_arguments(argc, argv, args_keywords, default_args);

  for( const auto &x : args ){
    if( x.first.back() == '=' ){
      std::cout << x.first << x.second << std::endl;
    }else{
      std::cout << x.first << ": " << x.second << std::endl;
    }
  }
  
  // Input from command line arguments.
  DAMCREM::Option option(args.at("--option="), true);
  DAMCREM::AppID app_id = DAMCREM::util::convert_string<DAMCREM::AppID>(args.at("--app-id="));
  DAMCREM::ClientID client_id = DAMCREM::util::convert_string<DAMCREM::ClientID>(args.at("--client-id="));
  bool is_pt = DAMCREM::util::convert_string<bool>(args.at("--pt"));
  bool is_ct = DAMCREM::util::convert_string<bool>(args.at("--ct"));
  size_t index = DAMCREM::util::convert_string<size_t>(args.at("--index="));
  bool is_same_value = (args.find("--scalar-value=") != args.end());
  size_t level = DAMCREM::util::convert_string<size_t>(args.at("--level="));
  std::string output_dir = args.at("--output-dir=");

  if( is_pt & is_ct ){ throw std::invalid_argument(""); }
  
  // Load scheme file.
  DAMCREM::SchemeList scheme_list(option);
  const DAMCREM::Scheme &scheme = scheme_list.scheme(app_id, client_id);

  // Set values
  size_t N = scheme.scheme_info().N();
  std::vector<double> values;
  if( !is_same_value ){
    values.resize(N, 0);
    std::cout << "Input " << N << " values: " << std::endl;
    for( size_t i = 0; i < N; ++i ){
      std::cin >> values.at(i);
    }
  }else{
    values.resize(N, std::stod(args.at("--scalar-value=")));
  }

  // Create target
  DAMCREM::Plaintext pt;
  DAMCREM::Ciphertext ct;
  scheme.encode(pt, values);
  pt.level = level;
  if( is_ct ){
    scheme.encrypt(ct, pt);
    ct.level = level;
  }

  // Save to a file.
  std::string target_path;

  if( is_pt ){
    if( output_dir == "const" ){
      target_path = option.file_path_config().const_data_file_path<DAMCREM::Plaintext>(app_id, client_id, index);
    }else if( output_dir == "input" ){
      target_path = option.file_path_config().input_data_file_path<DAMCREM::Plaintext>(app_id, client_id, index);
    }else{
      throw std::invalid_argument("Invalid output_dir");
    }
    scheme.save_data(pt, target_path);
  }else{
    if( output_dir == "const" ){
      target_path = option.file_path_config().const_data_file_path<DAMCREM::Ciphertext>(app_id, client_id, index);
    }else if( output_dir == "input" ){
      target_path = option.file_path_config().input_data_file_path<DAMCREM::Ciphertext>(app_id, client_id, index);
    }else{
      throw std::invalid_argument("Invalid output_dir");
    }
    scheme.save_data(ct, target_path);
  }
  
  return 0;
}

