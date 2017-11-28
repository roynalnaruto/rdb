#include <iostream>
#include "libdevcore/TrieDB.h"
#include "libdevcore/MemoryDB.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <typeinfo>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using namespace dev;

#include "classes/person.cpp"
#include "classes/template_db.cpp"
#include "utils/utils.cpp"

// MemoryDB db;
// GenericTrieDB<MemoryDB> t(&db);

typedef std::map<std::string, GenericTrieDB<MemoryDB>*> table_map;
typedef std::pair<std::string, GenericTrieDB<MemoryDB>*> table_map_pair;
table_map tables;

typedef std::map<std::string, int> table_col_info;
typedef std::pair<std::string, int> table_col_info_pair;
table_col_info tables_info;

#include "helpers/trie_db_interface.cpp"

// create sample table to start with
// create_table("sample_table", 2);
// insert(tables.at("sample_table"), 1, "rohit");
// std::cout<<lookup(tables.at("sample_table"), 1)<<std::endl;

#include "classes/node.cpp"

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
  // use different 't' for different tables?
  // t.init();

  create_table("sample_table", 2);
  insert(tables.at("sample_table"), 1, "rohit");
  std::cout<<lookup(tables.at("sample_table"), tables_info.at("sample_table"), 1)<<std::endl;

  boost::thread_group threads;
  threads.create_thread(accept_client_thread);
  threads.create_thread(thread_1);
  threads.create_thread(thread_2);
  threads.join_all();
  return 0;
}
