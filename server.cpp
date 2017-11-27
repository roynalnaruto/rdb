#include <iostream>
#include "TrieDB.h"
#include "MemoryDB.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <typeinfo>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using namespace dev;

MemoryDB db;
GenericTrieDB<MemoryDB> t(&db);

class Person {
  int id_;
  std::string name_;

public:
  void set_person(int, std::string);
  std::string get_person();
};

void Person::set_person(int id, std::string name) {
  id_ = id;
  name_ = name;
}

std::string Person::get_person() {
  std::string result("");
  result += std::to_string(id_);
  result += ", " + name_;
  return result;
}

/*
class Score {
  int id_;
  double score_;

public:
  void set_score(int, double);
};

void Score::set_score(int id, double score) {
  id_ = id;
  score_ = score;
}
*/

std::vector<unsigned char> int_to_bytes(int param) {
  std::vector<unsigned char> array_of_byte(4);
  for (int i = 0; i < 4; i++)
    array_of_byte[3 - i] = (param >> (i * 8));
  return array_of_byte;
}

std::vector<unsigned char> obj_to_bytes(Person* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::string bytes_to_string(char* buffer) {
  auto person_deserialized = reinterpret_cast<Person*>(buffer);
  return (*person_deserialized).get_person();
}

void insert(int id, std::string name) {
  Person person;
  person.set_person(id, name);
  t.insert(int_to_bytes(id), obj_to_bytes(&person));
}

std::string lookup(int id) {
  return bytes_to_string(&t.at(int_to_bytes(id))[0]);
}

struct node;
io_service service;
typedef boost::shared_ptr<node> node_ptr;
typedef std::vector<node_ptr> array;
array nodes;
boost::recursive_mutex cs;

struct node : boost::enable_shared_from_this<node> {
  node() : sock_(service) {
  }

  void serve_insert() {
    try {
      read_insert_query();
      process_insert_query();
    } catch(boost::system::system_error & err) {
      std::cout<<err.what()<<std::endl;
      stop();
    }
  }

  void serve_lookup() {
    std::string output;
    try {
      read_lookup_query();
      output = process_lookup_query();
      write(output + "\n");
    } catch(boost::system::system_error & err) {
      std::cout<<err.what()<<std::endl;
      stop();
    }
  }

  ip::tcp::socket & sock() {
    return sock_;
  }

  void stop() {
    boost::system::error_code err;
    sock_.close(err);
  }

private:
  void read_insert_query() {
    if (sock_.available()) {
      already_read_ += sock_.read_some(buffer(buff_ + already_read_, max_msg - already_read_));
    }
  }

  void read_lookup_query() {
    if (sock_.available()) {
      already_read_ += sock_.read_some(buffer(buff_ + already_read_, max_msg - already_read_));
    }
  }

  void process_insert_query() {
    bool found_endofline = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
    if (!found_endofline) {
      return;
    }
    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    int id = std::stoi(get_id(msg));
    std::string name = get_name(msg);

    boost::recursive_mutex::scoped_lock lk(cs);
    insert(id, name);
  }
  
  std::string process_lookup_query() {
    bool found_endofline = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
    if (!found_endofline) {
      return "\n";
    }
    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    int id = std::stoi(get_id(msg));

    boost::recursive_mutex::scoped_lock lk(cs);
    return lookup(id);
  }
 
  std::string get_id(std::string msg) {
    size_t found_delimeter;
    std::string id("-1");
    if ((found_delimeter = msg.find(":")) != std::string::npos) {
      id = msg.substr(0, found_delimeter);
    }
    return id;
  }

  std::string get_name(std::string msg) {
    size_t found_delimeter;
    std::string name("-1");
    if ((found_delimeter = msg.find(":")) != std::string::npos) {
      name =  msg.substr(found_delimeter+1, std::string::npos);
    }
    return name;
  }

  void write(const std::string & msg) {
    sock_.write_some(buffer(msg));
  }

private:
  ip::tcp::socket sock_;
  enum {max_msg = 1024};
  int already_read_;
  char buff_[max_msg];
};

void accept_client_thread() {
  ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
  while(true) {
    node_ptr new_(new node);
    acceptor.accept(new_->sock());
    boost::recursive_mutex::scoped_lock lk(cs);
    nodes.push_back(new_);
  }
}

void insert_thread() {
  while(true) {
    boost::this_thread::sleep(millisec(1));
    boost::recursive_mutex::scoped_lock lk(cs);
    for(array::iterator a = nodes.begin(), z = nodes.end(); a != z; ++a) {
      (*a)->serve_insert();
    }
  }
}

void lookup_thread() {
  while(true) {
    boost::this_thread::sleep(millisec(1));
    boost::recursive_mutex::scoped_lock lk(cs);
    for(array::iterator a = nodes.begin(), z = nodes.end(); a != z; ++a) {
      (*a)->serve_lookup();
    }
  }
}

int main() {
  t.init();
  /*
  insert(1, "rohit");
  insert(2, "mitali");
  std::cout<<lookup(1)<<std::endl;
  std::cout<<lookup(2)<<std::endl;
  */

  boost::thread_group threads;
  threads.create_thread(accept_client_thread);
  threads.create_thread(insert_thread);
  threads.create_thread(lookup_thread);
  threads.join_all();
  return 0;
}
