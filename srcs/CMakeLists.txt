cmake_minimum_required(VERSION 3.19)

add_executable(romisvr
	"romi.cpp"
	"error/errorutils.cpp"
	"error/exception.cpp"
	"error/runtimeexception.cpp"
	"error/socketexception.cpp"
	"error/invalidoperationexception.cpp"
	"server/io/socketclient.cpp"
	"utils/outpacket.cpp"
	"utils/inpacket.cpp"
	"utils/netutils.cpp"
)

if(MSVC)
	target_sources(romisvr
		PRIVATE
			server/io/win/iocp.cpp
			server/io/win/iocpthread.cpp
	)
else()
	target_sources(romisvr
		PRIVATE
			server/io/bsd/queuehost.cpp
			server/io/bsd/queuethread.cpp
	)
endif()

target_include_directories(romisvr
	PUBLIC ${CMAKE_SOURCE_DIR}/includes
)
