#include "pch.h"
#include "Buffer.h"


Buffer::Buffer() : m_chunkSize(g_defaultChunk), m_lastValue(g_defaultLastValue)
{
}

Buffer::Buffer(int chunk = g_defaultChunk, int lastValue = g_defaultLastValue) : m_chunkSize(chunk), m_lastValue(lastValue)
{
}


Buffer::~Buffer()
{

}

void Buffer::OutputData()
{
	std::thread dataThread(&Buffer::GenerateChunk, &*this, 0);
	OutputChunk();

	dataThread.join();
}

void Buffer::GenerateChunk(int startValue)
{
	int currentValue = startValue;
	while (currentValue <= m_lastValue)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (int i = 0; i < m_chunkSize; i++)
		{
			m_buffer.push_back(currentValue++);
		}

		m_condVar.notify_one();
	}
}

void Buffer::OutputChunk()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_condVar.wait(lock, [this] { return !m_buffer.empty(); });

	std::for_each(m_buffer.begin(), m_buffer.end(), [](int elem) { std::cout << elem << '\t'; });

	m_buffer.clear();
	lock.unlock();
}
