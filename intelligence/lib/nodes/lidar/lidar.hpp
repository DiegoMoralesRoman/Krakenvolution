#pragma once

#include "nodes/lidar/context.hpp"
#include "nodes/nodes.hpp"
#include "topics.hpp"

namespace nodes::lidar {
	INIT_NODE(lidar, lidar::Context);
}
