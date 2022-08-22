include_directories(
  ${CMAKE_BINARY_DIR}/generated/third_party/opentelemetry-proto
  ${CMAKE_SOURCE_DIR}/exporters/otlp/include)
  
add_executable(miv_otlp main.cc)
target_link_libraries(
  miv_otlp
  ${CMAKE_THREAD_LIBS_INIT}
  hashset
  opentelemetry_trace
  opentelemetry_logs
  opentelemetry_exporter_otlp_http
  opentelemetry_exporter_otlp_http_log
)
