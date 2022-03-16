cmake_minimum_required(VERSION 3.19)

add_executable(romisvr
	"romi.cpp"
	"win.cpp"
	"concurrent/interlocked.cpp"
	"error/errorutils.cpp"
	"error/exception.cpp"
	"error/runtimeexception.cpp"
	"error/socketexception.cpp"
	"error/invalidoperationexception.cpp"
	"server/server.cpp"
	"server/acceptor.cpp"
	"server/serverthread.cpp"
)

target_include_directories(romisvr
	PUBLIC ${CMAKE_SOURCE_DIR}/includes
)