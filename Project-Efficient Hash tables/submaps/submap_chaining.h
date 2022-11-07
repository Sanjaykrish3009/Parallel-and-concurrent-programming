#include "../hash_tables/hash_table_chaining.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <set>
#include <atomic>
#include <unistd.h>
#include <semaphore.h>
#include <bits/stdc++.h>

// class for the submap
class ht_submap : public hash_table_chaining{
private:
	// utility variables
	int num_threads_supported;
	thread* worker_threads;
	vector<thread> executer_threads;
	hash_table_chaining* sub_maps;
	queue<tuple<int,string,string,string>>* sub_map_queue;
	sem_t* semaphore_signal_main_to_thread;
	bool* semaphore_flags;
	bool* running_workers;
	int executes = 0;
	timer* timers;
	atomic<int> insert_time,delete_time,search_time;
	atomic<int> inserts,deletes,searches;
	set<int> ids;
	long total_time = 0;

public:
	bool record_time = false;
	// constructor
	ht_submap(int init_length_){
		init_length = init_length_;

		// set number of threads
		num_threads_supported = std::thread::hardware_concurrency();

		worker_threads = new thread[num_threads_supported];
		running_workers = new bool[num_threads_supported];

		sub_maps = new hash_table_chaining[num_threads_supported];
		sub_map_queue = new queue<tuple<int,string,string,string>>[num_threads_supported];

		semaphore_signal_main_to_thread = new sem_t[num_threads_supported];
		semaphore_flags = new bool[num_threads_supported];

		timers = new timer[num_threads_supported];

		for(int i = 0 ; i < num_threads_supported ; i++){
			semaphore_flags[i] = true;
			sem_init(&semaphore_signal_main_to_thread[i],1,0);
		}
	}

	// function which handles a thread
	void handle_thread(int id){
		sem_wait(&semaphore_signal_main_to_thread[id]);
		if(sub_map_queue[id].size()){
			tuple<int,string,string,string> operation = sub_map_queue[id].front();
			sub_map_queue[id].pop();
			int hash_value = get<0>(operation);
			if(get<1>(operation) == "insert"){
				timers[id].start_timer();
				sub_maps[id].insert(get<2>(operation),get<3>(operation),hash_value);
				timers[id].stop_timer();
				if(record_time) insert_time += timers[id].getTime();
			} else if(get<1>(operation) == "delete"){
				timers[id].start_timer();
				sub_maps[id].delete_item(get<2>(operation),hash_value);
				timers[id].stop_timer();
				if(record_time) delete_time += timers[id].getTime();
			} else if(get<1>(operation) == "search"){
				timers[id].start_timer();
				sub_maps[id].search(get<2>(operation),hash_value);
				timers[id].stop_timer();
				if(record_time) search_time += timers[id].getTime();
			}
			handle_thread(id);
		} else if(not semaphore_flags[id]){
			return;
		}
	}

	// function which executes the thread, creates new thread if not already running on specific ID
	void execute(string mode,string key,string value=""){
		int hash_value = get_hash_value(key);
		int id = hash_value%num_threads_supported;
		if(not running_workers[id]){
			worker_threads[id] = thread(&ht_submap::handle_thread,this,id);
			running_workers[id] = true;
		}
		sub_map_queue[id].push(make_tuple(hash_value,mode,key,value));
		sem_post(&semaphore_signal_main_to_thread[id]);
	}

	// utility function for application and tracking time
	void utility(string mode,string key,string value =""){
		usleep(10);
		thread executer(&ht_submap::execute,this,mode,key,value);
		executer_threads.emplace_back(std::move(executer));
	}

	// function which finishes all the threads, waits till all the threads are joined
	void complete_execution(){
		for(int i = 0 ; i < num_threads_supported ; i++){
			if(worker_threads[i].joinable()){
				semaphore_flags[i] = false;
				sem_post(&semaphore_signal_main_to_thread[i]);
				worker_threads[i].join();
			} 
		}

		for(int i = 0 ; i < num_threads_supported ; i++){
			sem_destroy(&semaphore_signal_main_to_thread[i]);
		}

		for(thread &executer : executer_threads){
			if(executer.joinable()){
				executer.join();
			}
		}
	}

	vector<int> get_times(){
		return {insert_time,delete_time,search_time};
	}

	~ht_submap(){
	}
};