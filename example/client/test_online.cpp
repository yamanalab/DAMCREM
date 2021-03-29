#include"communication/client/client.h"

int main(int argc, char *argv[]){
  DAMCREM::communication::Client client{};
  client.connect();

  auto engine = DAMCREM::util::create_random_generator();
  std::uniform_real_distribution<> dist(0.0, 5.0);
  double input_value = (argc > 1 ? DAMCREM::util::convert_string<double>(argv[1]) : dist(engine));
  std::cerr << "input_value = " << input_value << std::endl;

  DAMCREM::AppID app_id = 1;
  DAMCREM::ClientID client_id = 0;
  DAMCREM::SchemeInfo scheme_info("scheme/1/1000000000/dummy");
  DAMCREM::SchemeList scheme_list;
  // 必要に応じて引数を変えて，フォルダを指定する．
  DAMCREM::FilePathConfig fpc(DAMCREM::DirPathConfig("./", "app", "scheme", "const_data", "input"));
  DAMCREM::ClientID dummy_id = 1000000000;
  DAMCREM::QueryID query_id = client.get_new_query_id();
  std::cout << "query ID is " << query_id << std::endl;
  std::vector<DAMCREM::Plaintext> const_pt;
  std::vector<DAMCREM::Ciphertext> const_ct;
  std::vector<DAMCREM::Plaintext> input_pt;
  std::vector<DAMCREM::Ciphertext> input_ct;
  std::vector<DAMCREM::Plaintext> result_pt;
  std::vector<DAMCREM::Ciphertext> result_ct;
  
  // Request client ID
  client.request_client_id(client_id, app_id);
  std::cerr << std::endl;

  // Create scheme and register it to scheme_list
  scheme_list.add_scheme(app_id, client_id, DAMCREM::Scheme(scheme_info));
  const DAMCREM::Scheme &scheme = scheme_list.scheme(app_id, client_id);
  
  // Set scheme
  client.update_scheme(app_id, client_id, scheme.scheme_info());
  std::cerr << std::endl;

  auto create_and_send_query = [&](){
    // Create query
    std::vector<double> x(scheme.scheme_info().num_slot(), input_value);
    DAMCREM::Plaintext pt;
    input_ct.resize(1);
    scheme.encode(pt, x);
    scheme.encrypt(input_ct.at(0), pt);
    DAMCREM::communication::QueryC2S query(app_id, client_id, query_id, std::move(input_pt), std::move(input_ct), scheme_list);
    client.send_query(query);
    
    // Request result
    do{
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }while( !client.request_result(result_pt, result_ct, app_id, client_id, query.query_id, scheme) );
    std::cerr << result_pt.size() << ", " << result_ct.size() << std::endl;
    std::vector<double> v;
    DAMCREM::Plaintext p;
    for( const auto &x : result_pt ){
      scheme.decode(v, x);
      std::cout << v.at(0) << std::endl;
    }
    for( const auto &x : result_ct ){
      scheme.decrypt(p, x);
      scheme.decode(v, p);
      std::cout << v.at(0) << std::endl;
    }
    return;
  };

  create_and_send_query();
  std::cout << "correct value (e^x): " << std::exp(input_value) << std::endl;
  double result_taylor_expansion = 1.0, coeff = 1.0, x_i = 1.0;
  for( size_t i = 1; i < 8; ++i ){
    coeff /= i;
    x_i *= input_value;
    result_taylor_expansion += coeff * x_i;
  }
  std::cout << "correct value (TaylorExpansion(e^x, 7)): " << result_taylor_expansion << std::endl;
  std::cerr << std::endl;

    
  // Set const data by instance
  for( const std::filesystem::path &path : fpc.const_data_file_path_list<DAMCREM::Plaintext>(app_id, dummy_id, 8) ){
    const_pt.emplace_back(scheme.load_data<DAMCREM::Plaintext>(path));
  }
  client.update_const_data(app_id, client_id, const_pt, const_ct, scheme_list);
  std::cerr << std::endl;
  create_and_send_query();

  // Delete const data (Use default const value stored in the computation server)
  client.delete_const_data(app_id, client_id);
  create_and_send_query();
  
  
  // Delete my scheme from the server
  client.delete_scheme(app_id, client_id);
  std::cerr << std::endl;

  client.disconnect();
  std::cerr << std::endl;
  
  return 0;
}

