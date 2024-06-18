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
			pCurrentJobInfo = nullptr;
		}
	}

	ImGui::PlotHistogram("", frameTimes.data(), frameTimes.size(), 0, "Frame Samples", 0.0f, maxFrameBarValue, ImVec2(0.0f, 100.0f));
	
	ImGui::Spacing();
	ImGui::Spacing();
	
	DrawSequencer();

	ImGui::Spacing();
	ImGui::Spacing();

	DrawJobInfo();
	DrawRecommendations();
}

void JobWindow::DrawSequencer()
{
	// @TODO: Current problem: this gets drawn in the middle of the current frame! Collect the data, 
	// apply it at the end of the frame and display it beginning with the next frame.

	ImGui::SliderFloat("Zoom Level", &zoomFactor, 1.0f, 500.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
	ImGui::BeginChild("Sequencer", { 0, 0 }, true, ImGuiWindowFlags_HorizontalScrollbar);

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
			if (ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight)))
			{
				pCurrentJobInfo = &sample;
			}

			// Waited in between
			for (int i = 0; i < sample.profilingWaitData.size(); ++i)
			{
				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.5, 0.5, 1.0));

				duration = Time::GetDurationInMilliSec(sample.profilingWaitData[i].jobWaitEntryTime, sample.profilingWaitData[i].jobWaitExitTime);
				if (ImGui::DynamicTextButton("WaitForCounter", 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight)))
				{
					pCurrentJobInfo = &sample;
				}

				ImGui::PopStyleColor();

				ImGui::SameLine();

				if (sample.profilingWaitData.size() > 1 && i < sample.profilingWaitData.size())
				{
					duration = Time::GetDurationInMilliSec(sample.profilingWaitData[i].jobWaitExitTime, sample.profilingWaitData[i + 1].jobWaitEntryTime);
					if (ImGui::DynamicTextButton("WaitForCounter", 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight)))
					{
						pCurrentJobInfo = &sample;
					}

					ImGui::SameLine();
				}
			}

			duration = Time::GetDurationInMilliSec(sample.profilingWaitData.back().jobWaitExitTime, sample.jobExitTime);
			if (ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight)))
			{
				pCurrentJobInfo = &sample;
			}

			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, colors[Math::CRCHash::HashValue(sample.functionName.c_str()) % NUM_BAR_COLORS]);

			duration = Time::GetDurationInMilliSec(sample.jobEntryTime, sample.jobExitTime);
			if (ImGui::DynamicTextButton(sample.functionName.c_str(), 0, ImVec2(baseBarWidth * duration * zoomFactor, barHeight)))
			{
				pCurrentJobInfo = &sample;
			}

			ImGui::PopStyleColor();
		}

		// @TODO: Put all jobs in correct hierarchy!
	}
}

void JobWindow::DrawJobInfo()
{
	if (!pCurrentJobInfo) { return; }

	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	if (ImGui::BeginTable("Job Info", 2, flags))
	{
		ImGui::TableSetupColumn("Criteria", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Job Data", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
	
		{
			// Job info
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Job Description");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s: %s, %d", pCurrentJobInfo->fileName.c_str(), pCurrentJobInfo->functionName.c_str(), pCurrentJobInfo->lineNumber);
			}

			// -------------------------------------------------------------------------------

			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Starting Thread");

				ImGui::TableSetColumnIndex(1);
				int startingThreadID = pCurrentJobInfo->jobThreadId.size() > 0 ? pCurrentJobInfo->jobThreadId[0] : -1;
				
				if (startingThreadID > 0)
				{
					ImGui::Text("%d", startingThreadID);
				}
				else
				{
					ImGui::Text("Invalid!");
				}
			}

			// -------------------------------------------------------------------------------

			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Job Execution Time");

				ImGui::TableSetColumnIndex(1);

				float jobExecTime = Time::GetDurationInMilliSec(pCurrentJobInfo->jobEntryTime, pCurrentJobInfo->jobExitTime);
				if (jobExecTime >= 0) 
				{
					ImGui::Text("%.4f ms", jobExecTime);
				}
				else
				{
					ImGui::Text("Invalid!");
				}
			}
			

			// -------------------------------------------------------------------------------

			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Job Interruption Count");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", pCurrentJobInfo->profilingWaitData.size());
			}

			// -------------------------------------------------------------------------------

			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Total Job Wait Time");

				ImGui::TableSetColumnIndex(1);

				float totalWaitTime = 0.0f;
				for (auto& waitTime : pCurrentJobInfo->profilingWaitData)
					totalWaitTime += Time::GetDurationInMilliSec(waitTime.jobWaitEntryTime, waitTime.jobWaitExitTime);

				if (totalWaitTime >= 0)
				{
					ImGui::Text("%.4f ms", totalWaitTime);
				}
				else
				{
					ImGui::Text("Invalid!");
				}
			}

			// -------------------------------------------------------------------------------

			{

			}
		}

		ImGui::EndTable();
	}


	ImGui::TableNextColumn();

	


	/* @TDOD: Draw job info containing:
	* - Job description (Name, File, Line)
	* - Total job time
	* - Amount of waits 
	* - Total wait time
	* - Started thread
	* - Associated fiber
	* - ...
	*/
}

void JobWindow::DrawRecommendations()
{
	// @TODO: List recommendations like where to use spinning busy wait ...
}