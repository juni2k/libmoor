add_library(moor libmoor.c bitmap.c)
find_package(ZLIB)
target_link_libraries(moor ZLIB::ZLIB)