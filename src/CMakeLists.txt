add_executable(flex-pp
    CommandParser.cpp
    main.cpp
)

install(TARGETS flex-pp
    RUNTIME DESTINATION bin
)