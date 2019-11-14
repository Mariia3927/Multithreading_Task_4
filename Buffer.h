#pragma once
#include "pch.h"

class Buffer
{
public:
	Buffer();
	explicit Buffer(int chunk, int lastValue);
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;
	~Buffer();

	void OutputData();

private:
	void GenerateChunk(int startValue);
	void OutputChunk();

private:
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	std::vector<int> m_buffer;
	int m_chunkSize;
	int m_lastValue;
};

