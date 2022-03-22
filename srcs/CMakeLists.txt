cmake_minimum_required(VERSION 3.19)

add_executable(${PROJECT_NAME}
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
	target_sources(${PROJECT_NAME}
		PRIVATE
			server/io/win/iocp.cpp
			server/io/win/iocpthread.cpp
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC _ROMI_WINDOWS=1)
else()
	target_sources(${PROJECT_NAME}
		PRIVATE
			server/io/bsd/queuehost.cpp
			server/io/bsd/queuethread.cpp
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC _ROMI_WINDOWS=0)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR}/includes
)
