g++ server_variadic.cpp libdevcore/SHA3.cpp libdevcore/RLP.cpp libdevcore/MemoryDB.cpp libdevcore/TrieCommon.cpp libdevcore/CommonData.cpp libdevcore/Log.cpp -lboost_system -lboost_thread -lpthread -std=c++11 -o server_variadic

g++ client.cpp -std=c++11 -lboost_system -lboost_thread -o client
