set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})
include_directories ("./../src/")

set(SOURCES
	./../src/signature_builder.cpp
	../src/serializable_types.cpp
	./../src/hash.cpp
	./../src/delta_builder.cpp
	test_util.h)

add_executable (test test.cpp ${SOURCES})
target_link_libraries (test ${Boost_LIBRARIES})