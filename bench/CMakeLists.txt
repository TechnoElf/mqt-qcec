add_subdirectory("${PROJECT_SOURCE_DIR}/extern/benchmark" "extern/benchmark" EXCLUDE_FROM_ALL)

add_executable(
        ${PROJECT_NAME}_bench
        bench_diff_application_scheme.cpp)
target_link_libraries(${PROJECT_NAME}_bench PRIVATE ${PROJECT_NAME} benchmark::benchmark_main)
set_target_properties(${PROJECT_NAME}_bench PROPERTIES FOLDER bench)
