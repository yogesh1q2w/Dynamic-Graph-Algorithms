a.out:
	g++ -std=c++11 BinarySearchTree.cpp EulerTourTree.cpp connectivity.cpp MST.cpp SPT.cpp main.cpp

test:
	g++ -std=c++11 -o testall BinarySearchTree.cpp EulerTourTree.cpp connectivity.cpp MST.cpp SPT.cpp test.cpp
	./testall > temp_test
	if diff temp_test test_out.txt; then echo "PASS"; else echo "FAIL"; fi
	rm testall temp_test

clean:
	rm a.out