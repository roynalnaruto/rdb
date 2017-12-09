g++ tests/utils.cpp -lboost_system -std=c++1z -o tests/utils
g++ tests/template_db.cpp -lboost_system -std=c++1z -o tests/template_db
g++ tests/db_operations.cpp libdevcore/SHA3.cpp libdevcore/RLP.cpp libdevcore/MemoryDB.cpp libdevcore/TrieCommon.cpp libdevcore/CommonData.cpp libdevcore/Log.cpp -lboost_system -lboost_thread-mt -std=c++1z -o tests/db_operations
./tests/utils --log_level=test_suite
./tests/template_db --log_level=test_suite
./tests/db_operations --log_level=test_suite
