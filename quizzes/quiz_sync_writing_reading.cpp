/**************************************
 *	Developer: Eitan Bellaiche        *
 *	Date: 01/03/23	                  *
 *	Subject: SyncWritingReading 	  *
 **************************************/

#include <iostream> // std
#include <mutex>	// mutex
#include <atomic>	// atomic
#include <thread>	// thread, join
#include <chrono>	// seconds
#include <cstring>	// memcpy

using namespace std;

namespace ilrd
{
	class SyncWritingReading
	{
	public:
		SyncWritingReading();

		void StartRead();
		void EndRead();
		void StartWrite();
		void EndWrite();
	private:
		mutex m_mutex;
		atomic_int m_reading_cnt;
	};

	SyncWritingReading::SyncWritingReading(): m_reading_cnt(0) {}

	void SyncWritingReading::StartRead()
	{
		m_mutex.lock();
		++m_reading_cnt;
		m_mutex.unlock();
	}

	void SyncWritingReading::EndRead()
	{
		--m_reading_cnt;
	}

	void SyncWritingReading::StartWrite()
	{
		m_mutex.lock();
		while(0 < m_reading_cnt) {}
	}

	void SyncWritingReading::EndWrite()
	{
		m_mutex.unlock();
	}

} // namespace ilrd;

using namespace ilrd;

SyncWritingReading *swr = new SyncWritingReading();
const size_t arrSize = 20;
char testArr[arrSize] = "2020202020202020202";
size_t g_idx = 0;

void ReaderThread(int thrd_id)
{
	swr->StartRead();
	cout << "Reader Thread: " << thrd_id + 1 << ". Arr: "<< testArr << endl;
	swr->EndRead();
}

void WriterThread(int thrd_id)
{
	swr->StartWrite();
    testArr[g_idx++] = '1';
	cout << "Writer Thread: " << thrd_id + 1 << ". Arr: " << testArr << endl;
	swr->EndWrite();
}

int main(void)
{
	const int NUM_READERS = 20;
    const int NUM_WRITERS = 20;

	thread readers[NUM_READERS];
    thread writers[NUM_WRITERS];

	for (int i = 0; i < NUM_READERS; ++i)
    {
        readers[i] = thread(ReaderThread, i);
		this_thread::sleep_for(chrono::seconds(1));
		writers[i] = thread(WriterThread, i);
    }

    for (int i = 0; i < NUM_READERS; ++i)
    {
        readers[i].join();
    }

    for (int i = 0; i < NUM_WRITERS; ++i)
    {
        writers[i].join();
    }

    return 0;
}

