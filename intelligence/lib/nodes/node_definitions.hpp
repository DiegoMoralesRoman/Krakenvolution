#pragma once

// #include "topics/topics.hpp"
#include <nodes/node.hpp>
#include <nodes/nodes.hpp>

// Contexts
#include <nodes/robot/test/context.hpp>
#include <nodes/extensions/remote/context.hpp>

#define INIT_NODE(node_name, context_class, func_namespace) \
	auto setup(::core::topics::GlobalContext& global, context_class& ctx, const ::core::config::Config& cfg) -> void; \
	auto end(::core::topics::GlobalContext& global, context_class& ctx, const ::core::config::Config& cfg) -> void; \
	struct node_registration_##node_name { \
		node_registration_ ##node_name() { \
			static bool first_time = true; \
			if (first_time) { \
				first_time = false; \
				::core::nodes::static_pool().push_back(::core::nodes::ApplicationNode { \
						.node = ::core::nodes::create_node<context_class>(::core::func_namespace::node_name::setup, ::core::func_namespace::node_name::end), \
						.name = #node_name \
				}); \
			} \
		} \
	}; \
	static node_registration_ ##node_name node_name ##_registration __attribute__((used));

#define CREATE_NODE(name) \
	namespace core::nodes::name { INIT_NODE(name, Context, nodes) }
#define CREATE_EXTENSION(name) \
	namespace core::extensions::name { INIT_NODE(name, Context, extensions) }
