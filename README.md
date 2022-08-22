# opentelemetry-cpp-example
This example is based on existing [opentelemetry-cpp library](https://github.com/open-telemetry/opentelemetry-cpp).
It demonstrates how C++ app exports logs and traces to OpenTelemetryCollector with OTLP HTTP exporter and Zipkin receiver.

## Usage
1. Clone [opentelemetry-cpp library](https://github.com/open-telemetry/opentelemetry-cpp)
```console
git clone --recursive https://github.com/open-telemetry/opentelemetry-cpp
```
2. Copy-paste `hashset`, `miv_otlp` directories and `CMakeLists.txt` file into `opentelemetry-cpp/examples`
3. Build opentelemetry-cpp
```console
mkdir build && cd build
sudo cmake .. -DWITH_OTLP=ON -DENABLE_LOGS_PREVIEW=ON -DWITH_LOGS_PREVIEW=ON -DWITH_METRICS_PREVIEW=OFF -DENABLE_METRICS_PREVIEW=OFF
```
4. Run docker
```console
cd OpenTelemetryCollector
sudo docker-compose up
```
5. Start miv_otlp
```console
./examples/miv_otlp/miv_otlp http://localhost:4317/v1/traces
```
6. Observe `logs.json` in `OpenTelemetryCollector\output`
7. Observe in Zipkin
```console
http://127.0.0.1:9411/zipkin/
```
[image](https://user-images.githubusercontent.com/5527051/185891624-d2bc7cf0-f3a9-40f3-a8a6-598a04792457.png)
