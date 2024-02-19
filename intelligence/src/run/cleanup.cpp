#include "run.hpp"

#include <easylogging/easylogging++.h>
#include <google/protobuf/stubs/common.h>

auto run::shutdown_application() -> void {
	LOG(INFO) << "󰍛 Freeing memory...";
	google::protobuf::ShutdownProtobufLibrary();
}
