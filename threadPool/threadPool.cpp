#include "threadPool.h"

#include <iostream>
#include <chrono>
#include <mutex>

using namespace dpool;

std::mutex coutMtx;  // protect std::cout

int main(int argc, char* argv[])
{
	// max threads number is 100
	ThreadPool pool(100);

	// monitoring threads number for 13 seconds   
	auto monitorThreadPool = [&](int seconds) {
		for (int i = 1; i < seconds * 10; ++i)
		{
		    std::lock_guard<std::mutex> guard(coutMtx);
			std::cout << "thread num: " << pool.threadsNum() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	};
	pool.submit([&]() { monitorThreadPool(13); });

	auto threadTask = [](int taskId) {
		{
			std::lock_guard<std::mutex> guard(coutMtx);
			std::cout << "task-" << taskId << " begin!" << std::endl;
		}

		// executing task for 2 second
		std::this_thread::sleep_for(std::chrono::seconds(2));
		{
			std::lock_guard<std::mutex> guard(coutMtx);
			std::cout << "task-" << taskId << " end!" << std::endl;
		}
	};

	// submit 100 tasks
	for (int taskId = 0; taskId < 100; ++taskId)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		pool.submit([&]() {threadTask(taskId); });
	}
	return 0;
}
