#include"query.h"

namespace DAMCREM{

bool Query::read_from_stream(std::istream &stream){
  if( !stream ){ return false; }
  util::read(stream, &app_id_);
  util::read(stream, &client_id_);
  util::read(stream, &query_id_);
  util::read(stream, &job_id_);
  size_t n = 0;
  util::read(stream, &n);
  pt_.resize(n);
  for( size_t i = 0; i < n; ++i ){
    util::read(stream, &pt_.at(i));
  }
  util::read(stream, &n);
  ct_.resize(n);
  for( size_t i = 0; i < n; ++i ){
    util::read(stream, &ct_.at(i));
  }
  util::read(stream, &received_time_);
  util::read(stream, &deadline_);
  util::read(stream, &start_time_);
  util::read(stream, &finish_time_);
  return true;
}

bool Query::write_to_stream(std::ostream &stream) const {
  if( !stream ){ return false; }
  util::write(stream, &app_id_);
  util::write(stream, &client_id_);
  util::write(stream, &query_id_);
  util::write(stream, &job_id_);
  size_t n = pt_.size();
  util::write(stream, &n);
  std::for_each(pt_.begin(), pt_.end(), [&](const Plaintext &t){ util::write(stream, &t); });
  n = ct_.size();
  util::write(stream, &n);
  std::for_each(ct_.begin(), ct_.end(), [&](const Ciphertext &t){ util::write(stream, &t); });
  util::write(stream, &received_time_);
  util::write(stream, &deadline_);
  util::write(stream, &start_time_);
  util::write(stream, &finish_time_);
  return true;
}

size_t Query::stream_size() const {
  std::ostringstream oss;
  if( !write_to_stream(oss) ){ return 0; }
  return oss.tellp();
}

}

