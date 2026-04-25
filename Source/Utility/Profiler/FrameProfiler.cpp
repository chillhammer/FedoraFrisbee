#include <FedPCH.h>
#include "FrameProfiler.h"
#include <cstring>
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Fed
{
	FrameProfiler& FrameProfiler::Instance()
	{
		static FrameProfiler instance;
		return instance;
	}

	void FrameProfiler::BeginFrame()
	{
		const double now = NowMs();
		m_FrameStartMs = now;
		m_CurrentFrameIntervalMs = (m_LastFrameStartMs > 0.0) ? now - m_LastFrameStartMs : 0.0;
		m_LastFrameStartMs = now;
		m_CurrentDrawCalls = 0;
		m_CurrentTriangles = 0;

		if (m_ReportWindowStartMs <= 0.0)
			m_ReportWindowStartMs = now;

		for (Sample& sample : m_Samples)
		{
			sample.FrameMs = 0.0;
			sample.FrameCalls = 0;
			sample.Active = false;
		}
		m_OverflowSample.FrameMs = 0.0;
		m_OverflowSample.FrameCalls = 0;
		m_OverflowSample.Active = false;
	}

	void FrameProfiler::EndFrame()
	{
		const double now = NowMs();
		m_CurrentCpuFrameMs = now - m_FrameStartMs;
		++m_WindowFrameCount;
		m_WindowCpuMs += m_CurrentCpuFrameMs;
		m_WindowMaxCpuMs = std::max(m_WindowMaxCpuMs, m_CurrentCpuFrameMs);
		m_WindowDrawCalls += m_CurrentDrawCalls;
		m_WindowTriangles += m_CurrentTriangles;

		if (m_CurrentFrameIntervalMs > 0.0)
		{
			++m_WindowIntervalCount;
			m_WindowFrameIntervalMs += m_CurrentFrameIntervalMs;
			m_WindowMaxFrameIntervalMs = std::max(m_WindowMaxFrameIntervalMs, m_CurrentFrameIntervalMs);
		}

		if (now - m_ReportWindowStartMs >= m_ReportIntervalMs)
		{
			CaptureReport();
			ResetReportWindow();
			m_ReportWindowStartMs = now;
		}
	}

	void FrameProfiler::BeginSample(const char* name)
	{
		Sample& sample = FindOrCreateSample(name);
		if (sample.Active)
			return;

		sample.Active = true;
		sample.StartMs = NowMs();
	}

	void FrameProfiler::EndSample(const char* name)
	{
		Sample& sample = FindOrCreateSample(name);
		if (!sample.Active)
			return;

		const double elapsedMs = NowMs() - sample.StartMs;
		sample.Active = false;
		sample.FrameMs += elapsedMs;
		sample.WindowMs += elapsedMs;
		++sample.FrameCalls;
		++sample.WindowCalls;
	}

	void FrameProfiler::AddDrawCall(unsigned int indexCount)
	{
		++m_CurrentDrawCalls;
		m_CurrentTriangles += indexCount / 3;
	}

	double FrameProfiler::GetMetric(int metricId) const
	{
		if (metricId < 0 || metricId >= MetricCount)
			return 0.0;

		return m_LatestMetrics[metricId];
	}

	double FrameProfiler::NowMs() const
	{
		return glfwGetTime() * 1000.0;
	}

	FrameProfiler::Sample& FrameProfiler::FindOrCreateSample(const char* name)
	{
		for (int i = 0; i < m_SampleCount; ++i)
		{
			if (std::strcmp(m_Samples[i].Name, name) == 0)
				return m_Samples[i];
		}

		if (m_SampleCount < MaxSamples)
		{
			Sample& sample = m_Samples[m_SampleCount++];
			sample.Name = name;
			return sample;
		}

		m_OverflowSample.Name = "Profiler.Overflow";
		return m_OverflowSample;
	}

	const FrameProfiler::Sample* FrameProfiler::FindSample(const char* name) const
	{
		for (int i = 0; i < m_SampleCount; ++i)
		{
			if (std::strcmp(m_Samples[i].Name, name) == 0)
				return &m_Samples[i];
		}
		return nullptr;
	}

	double FrameProfiler::AverageSampleMs(const char* name) const
	{
		const Sample* sample = FindSample(name);
		if (!sample || m_WindowFrameCount == 0)
			return 0.0;

		return sample->WindowMs / static_cast<double>(m_WindowFrameCount);
	}

	void FrameProfiler::CaptureReport()
	{
		const double frameAvgMs = (m_WindowIntervalCount > 0)
			? m_WindowFrameIntervalMs / static_cast<double>(m_WindowIntervalCount)
			: 0.0;
		const double cpuAvgMs = (m_WindowFrameCount > 0)
			? m_WindowCpuMs / static_cast<double>(m_WindowFrameCount)
			: 0.0;
		const double fps = (frameAvgMs > 0.0) ? 1000.0 / frameAvgMs : 0.0;
		const double drawCallsAvg = (m_WindowFrameCount > 0)
			? static_cast<double>(m_WindowDrawCalls) / static_cast<double>(m_WindowFrameCount)
			: 0.0;
		const double trianglesAvg = (m_WindowFrameCount > 0)
			? static_cast<double>(m_WindowTriangles) / static_cast<double>(m_WindowFrameCount)
			: 0.0;

		m_LatestMetrics[0] = 1.0;
		m_LatestMetrics[1] = fps;
		m_LatestMetrics[2] = frameAvgMs;
		m_LatestMetrics[3] = m_CurrentFrameIntervalMs;
		m_LatestMetrics[4] = cpuAvgMs;
		m_LatestMetrics[5] = m_CurrentCpuFrameMs;
		m_LatestMetrics[6] = m_WindowMaxFrameIntervalMs;
		m_LatestMetrics[7] = m_WindowMaxCpuMs;
		m_LatestMetrics[8] = drawCallsAvg;
		m_LatestMetrics[9] = static_cast<double>(m_CurrentDrawCalls);
		m_LatestMetrics[10] = trianglesAvg;
		m_LatestMetrics[11] = static_cast<double>(m_CurrentTriangles);
		m_LatestMetrics[12] = AverageSampleMs("Frame.Clear");
		m_LatestMetrics[13] = AverageSampleMs("Game.Run");
		m_LatestMetrics[14] = AverageSampleMs("Game.State");
		m_LatestMetrics[15] = AverageSampleMs("Game.Window");
		m_LatestMetrics[16] = AverageSampleMs("Team.Update");
		m_LatestMetrics[17] = AverageSampleMs("Team.DrawWorld");
		m_LatestMetrics[18] = AverageSampleMs("Team.UI");
		m_LatestMetrics[19] = AverageSampleMs("Team.ScoreReset");
	}

	void FrameProfiler::ResetReportWindow()
	{
		m_WindowFrameIntervalMs = 0.0;
		m_WindowCpuMs = 0.0;
		m_WindowMaxFrameIntervalMs = 0.0;
		m_WindowMaxCpuMs = 0.0;
		m_WindowDrawCalls = 0;
		m_WindowTriangles = 0;
		m_WindowFrameCount = 0;
		m_WindowIntervalCount = 0;

		for (Sample& sample : m_Samples)
		{
			sample.WindowMs = 0.0;
			sample.WindowCalls = 0;
		}
		m_OverflowSample.WindowMs = 0.0;
		m_OverflowSample.WindowCalls = 0;
	}
}

#ifdef __EMSCRIPTEN__
extern "C" EMSCRIPTEN_KEEPALIVE double FedoraFrisbeeProfilerGetMetric(int metricId)
{
	return Fed::FrameProfiler::Instance().GetMetric(metricId);
}
#endif
