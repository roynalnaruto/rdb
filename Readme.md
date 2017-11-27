# relational db

using cpp-ethereum merkle patricia tree

## compile

`g++ server.cpp SHA3.cpp RLP.cpp MemoryDB.cpp TrieCommon.cpp CommonData.cpp Log.cpp -lboost_system -lboost_thread-mt -lpthread -std=c++11 -stdlib=libc++ -o server`
