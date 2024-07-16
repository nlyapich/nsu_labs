list(APPEND CPPCHECK_ARGS
    --enable=warning,style,performance,portability,unusedFunction
    --std=c++17
    --verbose
    --language=c++
    -DMAIN=main
    -I ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/test/*.cpp
)

add_custom_target(
    check
    COMMAND cppcheck ${CPPCHECK_ARGS}
    COMMENT "running cppcheck"
)
