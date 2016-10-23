cmake_minimum_required(VERSION 2.8)
PROJECT(convexhull)
add_library(convexhull conflictlist.cpp convexhull.cpp face.cpp hulledge.cpp vector.cpp vertex.cpp)
target_link_libraries (convexhull /usr/local/lib64/libstdc++.so)

