#pragma once
#include <array>

namespace Fed
{
	class FrameProfiler
	{
	public:
		static FrameProfiler& Instance();

		void BeginFrame();
		void EndFrame();
		void BeginSample(const char* name);
		void EndSample(const char* name);
		void AddDrawCall(unsigned int indexCount);
		double GetMetric(int metricId) const;

	private:
		struct Sample
		{
			const char* Name = nullptr;
			double StartMs = 0.0;
			double FrameMs = 0.0;
			double WindowMs = 0.0;
			int FrameCalls = 0;
			int WindowCalls = 0;
			bool Active = false;
		};

		static constexpr int MaxSamples = 32;
		static constexpr int MetricCount = 20;

		FrameProfiler() = default;
		double NowMs() const;
		Sample& FindOrCreateSample(const char* name);
		const Sample* FindSample(const char* name) const;
		double AverageSampleMs(const char* name) const;
		void CaptureReport();
		void ResetReportWindow();

		std::array<Sample, MaxSamples> m_Samples;
		std::array<double, MetricCount> m_LatestMetrics = {};
		Sample m_OverflowSample;
		int m_SampleCount = 0;

		double m_FrameStartMs = 0.0;
		double m_LastFrameStartMs = 0.0;
		double m_CurrentFrameIntervalMs = 0.0;
		double m_CurrentCpuFrameMs = 0.0;
		double m_ReportWindowStartMs = 0.0;
		double m_ReportIntervalMs = 500.0;

		double m_WindowFrameIntervalMs = 0.0;
		double m_WindowCpuMs = 0.0;
		double m_WindowMaxFrameIntervalMs = 0.0;
		double m_WindowMaxCpuMs = 0.0;
		unsigned int m_CurrentDrawCalls = 0;
		unsigned int m_CurrentTriangles = 0;
		unsigned int m_WindowDrawCalls = 0;
		unsigned int m_WindowTriangles = 0;
		int m_WindowFrameCount = 0;
		int m_WindowIntervalCount = 0;
	};

	class FrameProfileScope
	{
	public:
		explicit FrameProfileScope(const char* name)
			: m_Name(name)
		{
			FrameProfiler::Instance().BeginSample(m_Name);
		}

		~FrameProfileScope()
		{
			FrameProfiler::Instance().EndSample(m_Name);
		}

		FrameProfileScope(const FrameProfileScope&) = delete;
		FrameProfileScope& operator=(const FrameProfileScope&) = delete;

	private:
		const char* m_Name;
	};
}
