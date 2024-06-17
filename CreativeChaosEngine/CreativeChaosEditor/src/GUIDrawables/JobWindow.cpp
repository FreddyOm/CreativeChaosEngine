#include "JobWindow.h"
#include "CCEngine.h"

void JobWindow::OnGui()
{
	static std::vector<float> frameTimes(5000);
	
	if (sampling)
	{
		frameTimes.push_back(Time::deltaTime);
	}

	if(ImGui::Button(sampling ? "Stop Sampling" : (frameTimes.size() > 0 ? "Resume Sampling" : "Start Sampling")))
	{		
		sampling = !sampling;
		Debug::sampleJobs = sampling;

		if (!sampling)
		{
			maxFrameBarValue = 0.0001f;
			for (float frameTime : frameTimes)
			{
				if (maxFrameBarValue < frameTime)
					maxFrameBarValue = frameTime;
			}
			maxFrameBarValue += 0.0001f;

			frameProfilingCache = Debug::GetFrameProfilingData2();
			frameStart = Debug::frameStart;
		}
	}	

	if (!sampling && frameTimes.size() > 0)
	{
		ImGui::SameLine();
		
		if (ImGui::Button("Remove Samples"))
		{
			// Remove Samples
			frameTimes.clear();
		}
	}

	ImGui::PlotHistogram("", frameTimes.data(), frameTimes.size(), 0, "Frame Samples", 0.0f, maxFrameBarValue, ImVec2(0.0f, 100.0f));
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	DrawSequencer();
}

void JobWindow::DrawSequencer()
{
	// @TODO: Current problem: this gets drawn in the middle of the current frame! Collect the data, 
	// apply it at the end of the frame and display it beginning with the next frame.

	ImGui::SliderFloat("Zoom Level", &zoomFactor, 1.0f, 500.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
	ImGui::BeginChild("Sequencer", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Spacing();

	int threadNum = 0;
	for (auto& thread : frameProfilingCache)
	{
		DrawThreadSequence(threadNum, thread);
	}

	ImGui::EndChild();
}

void JobWindow::DrawThreadSequence(int& threadNum, std::pair<const DWORD, std::vector<CCE::Debug::JobProfilingData>>& thread)
{
	ImGui::Text("Thread %d", threadNum++);

	for (auto& sample : thread.second)
	{
		float duration = Time::GetDurationInMilliSec(frameStart, sample.jobEntryTime);
		ImGui::SetCursorPosX(zoomFactor * baseBarWidth * duration);

		if (sample.profilingWaitData.size() > 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, colors[Math::CRCHash::HashValue(sample.functionName.c_str()) % NUM_BAR_COLORS]);

			duration = Time::GetDurationInMilliSec(sample.jobEntryTime, sample.profilingWaitData[0].jobWaitEntryTime);
			ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight));

			// Waited in between
			for (int i = 0; i < sample.profilingWaitData.size(); ++i)
			{
				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.5, 0.5, 1.0));

				duration = Time::GetDurationInMilliSec(sample.profilingWaitData[i].jobWaitEntryTime, sample.profilingWaitData[i].jobWaitExitTime);
				ImGui::DynamicTextButton("WaitForCounter", 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight));

				ImGui::PopStyleColor();

				ImGui::SameLine();

				if (sample.profilingWaitData.size() > 1 && i < sample.profilingWaitData.size())
				{
					duration = Time::GetDurationInMilliSec(sample.profilingWaitData[i].jobWaitExitTime, sample.profilingWaitData[i + 1].jobWaitEntryTime);
					ImGui::DynamicTextButton("WaitForCounter", 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight));

					ImGui::SameLine();
				}
			}

			duration = Time::GetDurationInMilliSec(sample.profilingWaitData.back().jobWaitExitTime, sample.jobExitTime);
			ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight));

			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, colors[Math::CRCHash::HashValue(sample.functionName.c_str()) % NUM_BAR_COLORS]);

			duration = Time::GetDurationInMilliSec(sample.jobEntryTime, sample.jobExitTime);
			ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight));

			ImGui::PopStyleColor();
		}
	}
}
