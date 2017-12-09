g++ tests/utils.cpp -lboost_system -std=c++1z -o tests/utils
g++ tests/template_db.cpp -lboost_system -std=c++1z -o tests/template_db
./tests/utils --log_level=test_suite
./tests/template_db --log_level=test_suite
