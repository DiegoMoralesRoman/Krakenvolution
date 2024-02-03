#pragma once

#include "context.hpp"
#include "nodes/nodes.hpp"
#include "topics/topics.hpp"

namespace core::nodes::lidar {
	INIT_NODE(lidar, lidar::Context);
}
