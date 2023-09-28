all:
	g++ stacklinkedlist.cpp allocator.cpp gtest.cpp -lgtest -pthread -o tspa-gtest