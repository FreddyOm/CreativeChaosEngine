#pragma once
class LoadTest
{
public:
	LoadTest() = default;
	~LoadTest() = default;

	static void DoWork();
	static void DoWorkWithArgs(int iterations);

};
