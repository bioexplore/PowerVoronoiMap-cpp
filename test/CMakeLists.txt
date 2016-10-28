cmake_minimum_required(VERSION 2.8)
PROJECT(testConvexHull)
add_executable(testConvexHull testConvexHull.cpp ../conflictlist.cpp ../convexhull.cpp ../face.cpp ../hulledge.cpp ../vector.cpp ../vertex.cpp) 
link_directories(/usr/local/lib64)
target_link_libraries (testConvexHull /usr/local/lib64/libstdc++.so)

