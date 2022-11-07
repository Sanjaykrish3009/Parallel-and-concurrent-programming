#include "../interface/hash_table_chaining.h"
#include <mutex>

// lock for logging and debugging purpose
std::mutex log_lock;

// hash table class
class hash_table_chaining : public hash_table_chaining_interface{
protected:
	int init_length = 10001;
	int present_length = 0;
	int hash_a;
	int hash_b;
	timer hash_compute;
	void resize_vector(size_t size){
		this->hash_table_.resize(size);
	}

	// the hash function
	int hash_function(string key){
		long hasher = 0;
		for(char ch : key){
			hasher += ch * abs(ch-'a');
		}
		long hash_value = hash_a*hasher+hash_b;
		hash_value %= large_prime;
		// mimicing computationally expense
		usleep(10);
		return hash_value % present_length;
	}

	// triggering seed for the hash function
	void triggerSeed(){
		// srand(time(NULL));
		hash_a = rand()%large_prime;
		hash_b = rand()%large_prime;
	}

	// function which gives the chain at hashed_index
	vector<node>* get_chain(string key){
		hash_compute.start_timer();
		int hash_value = hash_function(key);
		hash_compute.stop_timer();
		vector<node>* chain = &hash_table_[hash_value].second;
		return chain;
	}

	// utility function which gives the chain at hashed_index
	vector<node>* get_chain(string key,int hash_value_){
		vector<node>* chain = &hash_table_[hash_value_].second;
		return chain;
	}

	// utility function for searching
	node* low_level_search(vector<node>* chain,string key){
		for(node &item : *chain){
			if(item.key == key){
				return &item;
			}
		}
		return nullptr;
	}

	node* low_level_search(vector<node>* chain,string key,int& position){
		for(node &item : *chain){
			if(item.key == key){
				return &item;
			}
			position++;
		}
		return nullptr;
	}

	// mix of the above two functions in one
	pair<node*,vector<node>*> utility_search(string key){
		vector<node>* chain = get_chain(key);
		node* item = low_level_search(chain,key);
		return {item,chain};
	}

	pair<node*,vector<node>*> utility_search(string key,int& position){
		vector<node>* chain = get_chain(key);
		node* item = low_level_search(chain,key,position);
		return {item,chain};
	}

	// for debugging purpose
	void log_item_count(){
		cout<<"[log] Item count in hashtable : "<<item_count<<endl;
	}

public:
	// constructors
	hash_table_chaining(){
		present_length = init_length;
		resize_vector(init_length);
		triggerSeed();
	}
	hash_table_chaining(int init_length_){
		// cout<<"Creating table with length : "<<init_length_<<endl;
		init_length = init_length_;
		present_length = init_length;
		resize_vector(init_length);
		triggerSeed();
	}

	// function which returns the hash value of a key
	int get_hash_value(string key){
		return hash_function(key);
	}

	// insert function in the hash table
	void insert(string key,string value){
		pair<node*,vector<node>*> pack = utility_search(key);
		node* item = pack.first;
		vector<node>* chain = pack.second;
		bool key_exists = item != nullptr;
		if(key_exists){
			item->value = value;
			// log_lock.lock();
			// cout<<"[Log] Key already exists! value overwritten. key:"<<key<<" value:"<<value<<endl;
			// log_lock.unlock();
		} else {
			node item(key,value);
			chain->emplace_back(item);
			// log_lock.lock();
			// cout<<"[Log] (Key,value) inserted. key:"<<key<<" value:"<<value<<endl;
			// log_lock.unlock();
			item_count++;
		}
		return;
	}

	void insert(string key,string value,int hash_value){
		vector<node>* chain = get_chain(key,hash_value);
		node* item = low_level_search(chain,key);
		bool key_exists = item != nullptr;
		if(key_exists){
			item->value = value;
			// log_lock.lock();
			// cout<<"[Log] Key already exists! value overwritten. key:"<<key<<" value:"<<value<<endl;
			// log_lock.unlock();
		} else {
			node item(key,value);
			chain->emplace_back(item);
			// log_lock.lock();
			// cout<<"[Log] (Key,value) inserted. key:"<<key<<" value:"<<value<<endl;
			// log_lock.unlock();
			item_count++;
		}
		return;
	}

	// delete function of the hash table
	bool delete_item(string key){
		int position = 0;
		pair<node*,vector<node>*> pack = utility_search(key,position);
		node* item = pack.first;
		vector<node>* chain = pack.second;

		if(item == nullptr){
			// log_lock.lock();
			// cout<<"[Log] Key not found to delete item. key:"<<key<<endl;
			// log_lock.unlock();
		} else {
			auto pos = chain->begin()+position;
			chain->erase(pos);
			// log_lock.lock();
			// cout<<"[Log] Key found and item deleted. key:"<<key<<endl;
			// log_lock.unlock();
			item_count--;
		}

		return item != nullptr;
	}

	bool delete_item(string key,int hash_value){
		int position = 0 ;
		vector<node>* chain = get_chain(key,hash_value);
		node* item = low_level_search(chain,key,position);

		if(item == nullptr){
		
		} else {
			auto pos = chain->begin()+position;
			chain->erase(pos);
			item_count--;
		}

		return item != nullptr;
	}

	// search function in hash table
	bool search(string key){
		pair<node*,vector<node>*> pack = utility_search(key);
		node* item = pack.first;
		// if(item != nullptr){
		// 	log_lock.lock();
		// 	cout<<"[Log] Key found. key:"<<key<<endl;
		// 	log_lock.unlock();
		// } else {
		// 	log_lock.lock();
		// 	cout<<"[Log] Key not found. key:"<<key<<endl;
		// 	log_lock.unlock();
		// }
		return item != nullptr;				
	}

	bool search(string key,int hash_value){
		vector<node>* chain = get_chain(key,hash_value);
		node* item = low_level_search(chain,key);
		return item != nullptr;				
	}

	std::string retrieve(string key){
		pair<node*,vector<node>*> pack = utility_search(key);
		node* item = pack.first;
		if(item != nullptr){
			return item->value;
		} else {
			return "C++:None";
		}
	}

	void check(){
		cout<<"Size - "<<hash_table_.size()<<endl;
	}

	// application utility function
	void execute(int &time,string mode,string key,string value = ""){
		timer t;
		if(mode == "insert"){
			t.start_timer();
			insert(key,value);
			t.stop_timer();
			time = t.getTime() - hash_compute.getTime();

		} else if(mode == "delete"){
			t.start_timer();
			delete_item(key);
			t.stop_timer();
			time = t.getTime()- hash_compute.getTime();;
		} else {
			t.start_timer();
			search(key);
			t.stop_timer();
			time = t.getTime()- hash_compute.getTime();;
		}
	}
};