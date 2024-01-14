#include "lidar.hpp"
#include "nodes/node.hpp"
#include "nodes/nodes.hpp"
#include <thread>

void reader_worker();

nodes::NodePtr nodes::lidar::create_node() {
	return core::create_node<lidar::Context>(setup, end);
}

void nodes::lidar::setup(core::GlobalContext& global, lidar::Context &ctx) {
	ctx.reader_thread = std::thread(reader_worker);
}

void nodes::lidar::end(::core::GlobalContext &global, lidar::Context &ctx) {
	ctx.reader_thread.join();
}

void reader_worker() {
}

