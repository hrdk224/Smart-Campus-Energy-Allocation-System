all:
	g++ -std=c++17 -Wall -Wextra main.cpp buildings.cpp allocation.cpp files.cpp -o campus_sim

demo: all
	./campus_sim --demo

clean:
	rm -f campus_sim allocation_report.txt allocations.dat
