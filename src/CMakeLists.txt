

add_executable(
    CursorLocker
	Main.cpp
	Monitors.cpp
	Processes.cpp
	ProgramArgs.cpp
)

target_include_directories(
    CursorLocker PUBLIC
	"include"
)

target_link_libraries(
	CursorLocker
	Wtsapi32.lib
)

set_property(
	TARGET CursorLocker PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)


