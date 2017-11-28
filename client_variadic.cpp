#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;
io_service service;

ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
ip::tcp::socket sock(service);
int already_read_;
enum { max_msg = 1024 };
char buff_[max_msg];
bool connected_ = false;

void hello_server() {
  std::string msg("hello\n");
  sock.write_some(buffer(msg));
  std::cout<<"written msg hello"<<std::endl;
}

size_t read_complete(const boost::system::error_code & err, size_t bytes) {
  if (err) return 0;
  already_read_ = bytes;
  bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
  return found ? 0 : 1;
}

void process_msg() {
  std::string msg(buff_, already_read_);
  std::cout<<msg<<std::endl;
}

void read_response() {
  already_read_ = 0;
  read(sock, buffer(buff_), boost::bind(read_complete, _1, _2));
  process_msg();
}

void send_insert_query(int id, std::string name) {
  std::string msg("insert/" + std::to_string(id) + ":" + name + "\n");
  sock.write_some(buffer(msg));
}

void send_lookup_query(int id) {
  std::string msg("lookup/" + std::to_string(id) + ":" + "\n");
  sock.write_some(buffer(msg));
}

void request_insert_data() {
  int input_id;
  std::string input_name;
  std::cout<<"id (int): ";
  std::cin>>input_id;
  std::cout<<"name (string): ";
  std::cin>>input_name;
  send_insert_query(input_id, input_name);
  read_response();
}

void request_lookup_data() {
  int input_id;
  std::cout<<"id (int): ";
  std::cin>>input_id;
  send_lookup_query(input_id);
  read_response();
}

std::string columns_as_string(std::vector<std::string> columns) {
  std::string result("");
  for(int i = 0; i<columns.size()-1; i++) {
    result += columns[i] + " ";
  }
  result += columns[columns.size()-1];
  return result;
}

void send_create_table_query(std::string msg) {
  sock.write_some(buffer(msg));
}

void request_create_table() {
  std::cout<<"enter columns of table (int, long, double, string) (space separated)\n: ";
  std::vector<std::string> columns;
  std::string column;
  while(std::cin>>column) {
    columns.push_back(column);
  }
  std::string columns_as_string = columns_as_string(columns);
  std::string msg = "createtable/" + columns_as_string + "\n";
  send_create_table_query(msg);
  read_response();
}

void run_client() {
  std::cout<<"Hello! Connecting..."<<std::endl;
  try {
    sock.connect(ep);
    hello_server();
    // read_response();
    connected_ = true;
  } catch(boost::system::system_error & err) {
    std::cout<<"client terminated: "<<err.what()<<std::endl;
  }
  int option;
  std::string msg_to_be_sent;
  while(true) {
    std::cout<<"Enter option:\n(1) create table\n(2) insert in table\n(3) lookup in table\n"<<std::endl;
    std::cout<<"> ";
    std::cin>>option;
    switch(option) {
      case 1:
        request_create_table();
        break;
      case 2:
	request_insert_data();
	break;
      case 3:
        request_lookup_data();
        break;
      default:
        break;
    }
  }
}

/*
void ping() {
  while(true) {
    boost::this_thread::sleep(boost::posix_time::millisec(100));
    if (connected_) {
      std::string msg ("ping\n");
      sock.write_some(buffer(msg));
    }
  }
}
*/

int main(int argc, char* argv[]) {
  boost::thread_group threads;
  threads.create_thread(run_client);
  // threads.create_thread(ping);
  threads.join_all();
}
