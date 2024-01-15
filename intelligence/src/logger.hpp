/**
 * @file logger.hpp
 * @brief This file provides a basic configuration for the application logger
 */

#pragma once

#include "easylogging/easylogging++.h"
#include <string>

namespace run::log {
	/**
	 * @brief Generates default configuration for easylogger
	 * @returns Default configuration
	 * @details
	 * 	I would recommend this configuration to be extended outside if there is need to do so
	 * @todo Should add a source file for the function implementation
	 */
	el::Configurations get_config() {
		el::Configurations conf;

		conf.setToDefault();
		conf.set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level %msg");
		conf.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
		conf.set(el::Level::Global, el::ConfigurationType::ToFile, "false");
		conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
		conf.set(el::Level::Global, el::ConfigurationType::SubsecondPrecision, "3");
		conf.set(el::Level::Global, el::ConfigurationType::PerformanceTracking, "false");
		conf.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2097152");
		conf.set(el::Level::Global, el::ConfigurationType::LogFlushThreshold, "100");

		// Set colored logs
		conf.set(el::Level::Info, el::ConfigurationType::Format, "\x1B[32m%datetime %level %msg\x1B[0m"); // Green for Info
		conf.set(el::Level::Warning, el::ConfigurationType::Format, "\x1B[33m%datetime %level %msg\x1B[0m"); // Yellow for Warning
		conf.set(el::Level::Error, el::ConfigurationType::Format, "\x1B[31m%datetime %level %msg\x1B[0m"); // Red for Error
		conf.set(el::Level::Debug, el::ConfigurationType::Format, "\x1B[34m%datetime %level %msg\x1B[0m"); // Blue for Debug
		conf.set(el::Level::Fatal, el::ConfigurationType::Format, "\x1B[35m%datetime %level %msg\x1B[0m"); // Magenta for Fatal

		return conf;
	}
}
