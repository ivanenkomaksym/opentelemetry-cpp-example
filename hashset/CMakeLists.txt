add_library(hashset hashset.h hashset.cc)
target_link_libraries(hashset PUBLIC ${CMAKE_THREAD_LIBS_INIT}
                                                opentelemetry_api)
