#include "submap_chaining.h"

int main(){

	int testcases;
	cin>>testcases;
	//creating hash table using chaining submap
	ht_submap* hash_table = new ht_submap(testcases/2);

	timer t;

	std::string mode;
	int searches = 0,inserts = 0,deletes = 0;
	int search_time = 0,insert_time = 0,delete_time = 0;

   //Loading the hash table 
    while(testcases--){
        cin>>mode;
        if(mode == "insert"){
            string key,value;
            cin>>key>>value;
            hash_table->execute(mode,key,value);
        } else if(mode == "delete"){
            string key;
            cin>>key;
            hash_table->execute(mode,key);
        } else if(mode == "search"){
            string key;
            cin>>key;
            hash_table->execute(mode,key);
        }
    }
	sleep(10);
    cin>>testcases;
	int record = testcases;
	hash_table->record_time = true;

	t.start_timer();
	//checking for lookups and updates in hashtable
	while(testcases--){
		cin>>mode;
		if(mode == "insert"){
			string key,value;
			cin>>key>>value;
			hash_table->utility(mode,key,value);
			inserts++;
		} else if(mode == "delete"){
			string key;
			cin>>key;
			hash_table->utility(mode,key);
			deletes++;
		} else if(mode == "search"){
			string key;
			cin>>key;
			hash_table->utility(mode,key);
			searches++;
		}
	}
	hash_table->complete_execution();
	t.stop_timer();
	vector<int> times = hash_table->get_times();
	delete hash_table;

	int avg_reduces = 0;
	int iter_count = 100;
	for(int i = 0 ; i < iter_count ; i++){
		timer t;
		t.start_timer();
		usleep(5);
		t.stop_timer();
		avg_reduces += t.getTime();
	}
	avg_reduces /= iter_count;

	insert_time = times[0];
	delete_time = times[1];
	search_time = times[2];

	int updates = inserts + deletes;
	int lookups = searches;

	int update_time = insert_time + delete_time;
	int lookup_time = search_time;

	// cout<<"Total time for all updates and lookups("<<updates+lookups<<") : "<<t.getTime()-record*avg_reduces<<"us"<<endl;

	cout<<"Total time taken for updates("<<updates<<") : "<<update_time<<" us"<<endl;
	cout<<"Avg time taken for update operation "<<update_time/updates<<" us"<<endl;

	cout<<"Total time taken for lookups("<<lookups<<") : "<<lookup_time<<" us"<<endl;
	cout<<"Avg time taken for lookup operation "<<lookup_time/lookups<<" us"<<endl;
	return 0;
}