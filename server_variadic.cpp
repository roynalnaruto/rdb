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

std::string EMPTY_STR("\n");

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
  if (t.contains(int_to_bytes(id))) {
    return bytes_to_string(&t.at(int_to_bytes(id))[0]);
  } else {
    return "\n";
  }
}

struct node;
io_service service;
typedef boost::shared_ptr<node> node_ptr;
typedef std::vector<node_ptr> array;
array nodes;
boost::recursive_mutex cs;

struct node : boost::enable_shared_from_this<node> {
  node() : sock_(service), output_(EMPTY_STR) {
  }

  void serve_node() {
    try {
      read_query();
      process_query();
    } catch(boost::system::system_error & err) {
      std::cout<<err.what()<<std::endl;
      stop();
    }
  }

  /*
  void serve_lookup() {
    std::string output;
    try {
      read_lookup_query();
      process_lookup_query();
    } catch(boost::system::system_error & err) {
      std::cout<<err.what()<<std::endl;
      stop();
    }
  }
  */

  ip::tcp::socket & sock() {
    return sock_;
  }

  void stop() {
    boost::system::error_code err;
    sock_.close(err);
  }

private:
  void read_query() {
    if (sock_.available()) {
      already_read_ += sock_.read_some(buffer(buff_ + already_read_, max_msg - already_read_));
    }
  }

  /*
  void read_lookup_query() {
    if (sock_.available()) {
      already_read_ += sock_.read_some(buffer(buff_ + already_read_, max_msg - already_read_));
    }
  }
  */

  void process_query() {
    bool found_endofline = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
    if (!found_endofline) {
      return;
    }
    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    if (msg.find("createtable") == 0) {
      std:string query = parse(msg);
      handle_create_table_query(query);
    } else if (msg.find("insert") == 0) {
      int id = std::stoi(get_id(parse(msg)));
      std::string name = get_name(parse(msg));
      boost::recursive_mutex::scoped_lock lk(cs);
      std::cout<<"inserting "<<id<<", "<<name<<std::endl;
      insert(id, name);
      write("*INSERTED* " + std::to_string(id) + "->" + name + "\n");
    } else if (msg.find("lookup") == 0) {
      int id = std::stoi(get_id(parse(msg)));       
      boost::recursive_mutex::scoped_lock lk(cs);
      std::cout<<"looking up "<<id<<std::endl;
      output_ = lookup(id);
      if (EMPTY_STR != output_) {
        write("*LOOKUP* " + output_ + "\n");
      } else {
        write("*KEY NOT FOUND*\n");
      }
      output_ = "\n";
    }
  }
  
  void handle_create_table_query(std::string query) {
    
  }

  /*
  std::string process_lookup_query() {
    bool found_endofline = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
    if (!found_endofline) {
      return "\n";
    }
    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    if (msg.find("lookup") == 0) {
      int id = std::stoi(get_id(parse(msg)));

      boost::recursive_mutex::scoped_lock lk(cs);
      std::cout<<"looking up "<<id<<std::endl;
      return lookup(id);
    } else {
      return "\n";
    }
  }
  */

  std::string parse(std::string msg) {
    if (msg.find("/") != -1) {
      return msg.substr(msg.find("/")+1, std::string::npos);
    }
    return "_";
  }
 
  std::string get_id(std::string msg) {
    size_t found_delimeter;
    std::string id("-1");
    if ((found_delimeter = msg.find(":")) != std::string::npos) {
      id = msg.substr(0, found_delimeter);
    }
    // std::cout<<"id found: "<<id<<std::endl;
    return id;
  }

  std::string get_name(std::string msg) {
    size_t found_delimeter;
    std::string name("-1");
    if ((found_delimeter = msg.find(":")) != std::string::npos) {
      name =  msg.substr(found_delimeter+1, std::string::npos);
    }
    // std::cout<<"name found: "<<name<<std::endl;
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
  std::string output_;
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

void thread_1() {
  while(true) {
    boost::this_thread::sleep(millisec(1));
    boost::recursive_mutex::scoped_lock lk(cs);
    for(array::iterator a = nodes.begin(), z = nodes.end(); a != z; ++a) {
      (*a)->serve_node();
    }
  }
}

void thread_2() {
  while(true) {
    boost::this_thread::sleep(millisec(1));
    boost::recursive_mutex::scoped_lock lk(cs);
    for(array::iterator a = nodes.begin(), z = nodes.end(); a != z; ++a) {
      (*a)->serve_node();
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
  threads.create_thread(thread_1);
  threads.create_thread(thread_2);
  threads.join_all();
  return 0;
}
