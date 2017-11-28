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

  void process_query() {
    bool found_endofline = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
    if (!found_endofline) {
      return;
    }
    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    if (msg.find("create-table") == 0) {
      int n_columns = std::stoi(get_id(parse(msg)));
      std::string table_name = get_name(parse(msg));
      std::cout<<"creating table: "<<table_name<<" with "<<n_columns<<" columns"<<std::endl;
      create_table(table_name, n_columns);
      write("*CREATED TABLE*" + table_name + "\n");
    } else if (msg.find("insert") == 0) {
      int id = std::stoi(get_id(parse(msg)));
      std::string name = get_name(parse(msg));
      boost::recursive_mutex::scoped_lock lk(cs);
      std::cout<<"inserting "<<id<<", "<<name<<std::endl;
      // insert(id, name);
      write("*INSERTED* " + std::to_string(id) + "->" + name + "\n");
    } else if (msg.find("lookup") == 0) {
      int id = std::stoi(get_id(parse(msg)));       
      boost::recursive_mutex::scoped_lock lk(cs);
      std::cout<<"looking up "<<id<<std::endl;

      //
      table_name_ = "sample_table";
      //

      output_ = lookup(tables.at(table_name_), tables_info.at(table_name_), id);
      if (EMPTY_STR != output_) {
        write("*LOOKUP* " + output_ + "\n");
      } else {
        write("*KEY NOT FOUND*\n");
      }
      output_ = "\n";
    }
  }
  
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
  std::string output_;
  std::string table_name_;
};
