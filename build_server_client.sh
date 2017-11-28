g++ server_variadic.cpp libdevcore/SHA3.cpp libdevcore/RLP.cpp libdevcore/MemoryDB.cpp libdevcore/TrieCommon.cpp libdevcore/CommonData.cpp libdevcore/Log.cpp -lpthread -lboost_system -lboost_thread-mt -std=c++1z -stdlib=libc++ -o server_variadic

g++ client_variadic.cpp -std=c++11 -stdlib=libc++ -lboost_system -lboost_thread-mt -lpthread -o client_variadic
