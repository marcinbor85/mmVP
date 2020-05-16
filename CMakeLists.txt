cmake_minimum_required( VERSION 3.0 )

project( libcat LANGUAGES C )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

enable_testing( )

include_directories( ${PROJECT_SOURCE_DIR}/src )

file( GLOB SRC_FILES src/*.c )
add_library( mmvp SHARED ${SRC_FILES} )
set_target_properties( mmvp PROPERTIES VERSION 0.1.0 SOVERSION 1 )
target_compile_options( mmvp PRIVATE -Werror -Wall -Wextra -pedantic )

install( TARGETS mmvp DESTINATION lib )
install( FILES src/mmvp.h DESTINATION include/mmvp )

file( GLOB TEST_COMMON_SRC_FILES tests/common.c )
add_library( test_common SHARED ${TEST_COMMON_SRC_FILES} )

add_executable( test_init tests/test_init.c )
target_link_libraries( test_init mmvp test_common)
add_test( test_init ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_init )

add_executable( test_register tests/test_register.c )
target_link_libraries( test_register mmvp test_common)
add_test( test_register ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_register )

add_executable( test_unregister tests/test_unregister.c )
target_link_libraries( test_unregister mmvp test_common)
add_test( test_unregister ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_unregister )

add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} --verbose )
add_custom_target( cleanall COMMAND rm -rf Makefile CMakeCache.txt CMakeFiles/ bin/ lib/ cmake_install.cmake CTestTestfile.cmake Testing/ )
add_custom_target( uninstall COMMAND xargs rm < install_manifest.txt )
