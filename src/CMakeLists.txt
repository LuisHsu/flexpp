add_executable(flex-pp
    CommandParser.cpp
)

install(TARGETS flex-pp
    RUNTIME DESTINATION bin
)