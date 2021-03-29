#pragma once



namespace DAMCREM{
class ServerInfo{
public:
  ServerInfo(const std::string &host="localhost", const std::string &port="45678")
    : host_(host), port_(port){}
  ~ServerInfo() = default;
  ServerInfo(const ServerInfo &src) noexcept = default;
  ServerInfo(ServerInfo &&src) noexcept = default;

  ServerInfo& operator=(const ServerInfo &src){
    host_ = src.host_;
    port_ = src.port_;
    return *this;
  }
  
  const std::string& host() const noexcept { return host_; }
  const std::string& port() const noexcept { return port_; }
  
private:
  std::string host_;
  std::string port_;
};



}


