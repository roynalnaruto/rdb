/**
 * node class
 */
struct node : boost::enable_shared_from_this<node> {
  node() : sock_(service) {
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

    if (msg.find("createtable") == 0) {
      std::string table_name = create_table_helper(parse_to_clean(msg));
      write("*CREATED TABLE* " + table_name + "\n");
    } else if (msg.find("insert") == 0) {
      std::string response = insert_row_helper(parse_to_clean(msg));
      write("*INSERTED* " + response + "\n");
    } else if (msg.find("lookup") == 0) {
      std::string response = lookup_helper(parse_to_clean(msg));
      if (EMPTY_STR != response) {
        write("*LOOKUP* " + response + "\n");
      } else {
        write("*KEY NOT FOUND*\n");
      }
    } else if (msg.find("listtables") == 0) {
      std::string response = list_tables_helper();
      write("*LIST TABLES* " + response + "\n");
    }
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
