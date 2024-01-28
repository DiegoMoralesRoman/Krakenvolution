#pragma once

#include "nodes/lidar/context.hpp"
#include "nodes/nodes.hpp"
#include "config/config.hpp"
#include "topics.hpp"

namespace core::nodes::lidar {
	INIT_NODE(lidar, lidar::Context);
}
