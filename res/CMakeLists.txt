add_library(mc2res SHARED libmain.cpp libmainc.cpp)
target_include_directories(mc2res PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/../test_scripts/res_conv)
