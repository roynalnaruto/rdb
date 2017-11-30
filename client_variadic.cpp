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

void send_insert_query(std::string msg) {
  std::cout<<"sending query: "<<msg<<std::endl;
  sock.write_some(buffer(msg));
}

void send_lookup_query(std::string msg) {
  sock.write_some(buffer(msg));
}

void request_insert_data() {
  std::string table_name;
  int id;
  std::cout<<"Enter table name to insert to: ";
  std::cin>>table_name;
  std::cout<<"Enter row 'id': ";
  std::cin>>id;
  std::string msg = "insert/" + std::to_string(id) + ":" + table_name;
  std::cout<<"Enter columns (semi-colon separated) (min. 1, max. 4): ";
  std::string c_val;
  std::cin>>c_val;
  msg += ";" + c_val + "\n";
  send_insert_query(msg);
  read_response();
}

void request_lookup_data() {
  std::string table_name;
  std::cout<<"Enter table name to lookup in: ";
  std::cin>>table_name;
  int input_id;
  std::cout<<"Enter 'id' field (int): ";
  std::cin>>input_id;
  std::string msg = "lookup/" + std::to_string(input_id) + ":" + table_name + "\n";
  send_lookup_query(msg);
  read_response();
}

void send_create_table_query(std::string msg) {
  sock.write_some(buffer(msg));
}

void request_create_table() {
  std::string table_name;
  int n_col;
  std::cout<<"Enter name of table: ";
  std::cin>>table_name;
  std::cout<<"\nEnter number of columns (min. 2, max. 5): ";
  std::cin>>n_col;
  std::string msg = "createtable/" + std::to_string(n_col) + ":" + table_name + "\n";
  send_create_table_query(msg);
  read_response();
}

void send_list_tables_query(std::string msg) {
  sock.write_some(buffer(msg));
}

void request_list_tables() {
  std::string msg("listtables");
  msg += "\n";
  send_list_tables_query(msg);
  read_response();
}

void run_client() {
  std::cout<<"Hello! Connecting..."<<std::endl;
  try {
    sock.connect(ep);
    hello_server();
    connected_ = true;
  } catch(boost::system::system_error & err) {
    std::cout<<"client terminated: "<<err.what()<<std::endl;
  }
  int option;
  std::string msg_to_be_sent;
  while(true) {
    std::cout<<"Enter option:\n(1) create table\n(2) insert in table\n(3) lookup in table\n(4) list all tables\n"<<std::endl;
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
      case 4:
        request_list_tables();
        break;
      default:
        break;
    }
  }
}

int main(int argc, char* argv[]) {
  boost::thread_group threads;
  threads.create_thread(run_client);
  threads.join_all();
}
