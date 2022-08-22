// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifdef ENABLE_LOGS_PREVIEW
#  include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#  include "opentelemetry/exporters/otlp/otlp_http_log_exporter_factory.h"
#  include "opentelemetry/exporters/otlp/otlp_http_log_exporter_options.h"
#  include "opentelemetry/logs/provider.h"
#  include "opentelemetry/sdk/logs/logger_provider_factory.h"
#  include "opentelemetry/sdk/logs/simple_log_processor_factory.h"
#  include "opentelemetry/sdk/trace/simple_processor_factory.h"
#  include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#  include "opentelemetry/trace/provider.h"

#  include <string>

#  include "hashset.h"

namespace trace     = opentelemetry::trace;
namespace nostd     = opentelemetry::nostd;
namespace otlp      = opentelemetry::exporter::otlp;
namespace logs_sdk  = opentelemetry::sdk::logs;
namespace logs      = opentelemetry::logs;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace resource  = opentelemetry::sdk::resource;

namespace
{

opentelemetry::exporter::otlp::OtlpHttpExporterOptions opts;
void InitTracer()
{
  // Create OTLP exporter instance
  auto exporter  = otlp::OtlpHttpExporterFactory::Create(opts);
  auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));
  // Set the global trace provider
  trace::Provider::SetTracerProvider(provider);
}

opentelemetry::exporter::otlp::OtlpHttpLogExporterOptions logger_opts;
void InitLogger()
{
  logger_opts.console_debug = true;
  // Create OTLP exporter instance
  auto exporter  = otlp::OtlpHttpLogExporterFactory::Create(logger_opts);
  auto processor = logs_sdk::SimpleLogProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<logs::LoggerProvider> provider =
      logs_sdk::LoggerProviderFactory::Create(std::move(processor));

  opentelemetry::logs::Provider::SetLoggerProvider(provider);
}

}

int main(int argc, char *argv[])
{
  opts.url        = argv[1];
  logger_opts.url = argv[1];
  if (argc > 1)
  {
    opts.url        = argv[1];
    logger_opts.url = argv[1];
    if (argc > 2)
    {
      std::string debug  = argv[2];
      opts.console_debug = debug != "" && debug != "0" && debug != "no";
    }

    if (argc > 3)
    {
      std::string binary_mode = argv[3];
      if (binary_mode.size() >= 3 && binary_mode.substr(0, 3) == "bin")
      {
        opts.content_type        = opentelemetry::exporter::otlp::HttpRequestContentType::kBinary;
        logger_opts.content_type = opentelemetry::exporter::otlp::HttpRequestContentType::kBinary;
      }
    }
  }
  // Removing this line will leave the default noop TracerProvider in place.
  InitLogger();
  InitTracer();
  
  hashset_test();
}
#else
int main()
{
  return 0;
}
#endif
