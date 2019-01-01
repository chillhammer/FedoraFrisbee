#pragma once
#include <FedPCH.h>
#include <Macros.h>
#include "spdlog/spdlog.h"

namespace Fed {

	template class std::shared_ptr<spdlog::logger>;

	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

	//Core
#define LOG(...)			::Fed::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		::Fed::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		::Fed::Log::GetLogger()->error(__VA_ARGS__)

}
