cmake_minimum_required(VERSION 3.19)

add_executable(romisvr
	"romi.cpp"
	"error/errorutils.cpp"
	"error/exception.cpp"
	"error/runtimeexception.cpp"
	"error/socketexception.cpp"
	"error/invalidoperationexception.cpp"
	"utils/outpacket.cpp"
	"utils/inpacket.cpp"
)

target_include_directories(romisvr
	PUBLIC ${CMAKE_SOURCE_DIR}/includes
)
