#include "hash_table_openaddressing.h"

int main(){

    timer t;
    int testcases;
    cin>>testcases;
    //creating hash table using openaddressing
    hash_table_openaddr* ht_openaddr = new hash_table_openaddr(testcases);

    std::string mode;
    int searches = 0,inserts = 0,deletes = 0;
    int search_time = 0,insert_time = 0,delete_time = 0;
     
    int time=0;
    //Loading the hash table 
    while(testcases--){
        cin>>mode;
        time =0;
        if(mode == "insert"){
            string key,value;
            cin>>key>>value;
            ht_openaddr->execute(time,mode,key,value);
        } else if(mode == "delete"){
            string key;
            cin>>key;
            ht_openaddr->execute(time,mode,key);
        } else if(mode == "search"){
            string key;
            cin>>key;
            ht_openaddr->execute(time,mode,key);
        }
    }
    cin>>testcases;
	t.start_timer();
    //checking for lookups and updates in hashtable
    while(testcases--){
        cin>>mode;
        if(mode == "insert"){
            string key,value;
            cin>>key>>value;
            time = 0;
            ht_openaddr->execute(time,mode,key,value);
            inserts++;
            insert_time += time;
        } else if(mode == "delete"){
            string key;
            cin>>key;
            time = 0;
            ht_openaddr->execute(time,mode,key);
            deletes++;
            delete_time += time;
        } else if(mode == "search"){
            string key;
            cin>>key;
            time = 0;
            ht_openaddr->execute(time,mode,key);
            searches++;
            search_time += time;
        }
    }
    t.stop_timer();

    int updates = inserts+deletes;
    int lookups = searches;
	// cout<<"Total time for all updates and lookups("<<updates+lookups<<") : "<<t.getTime()<<"us"<<endl;
    int updates_time = insert_time+delete_time;
    cout<<"Total time taken for updates("<<updates<<""<<") : "<<updates_time<<" us"<<endl;
    cout<<"Avg time taken for update operation "<<updates_time/updates<<" us"<<endl;
    
    int lookups_time = search_time;
    cout<<"Total time taken for lookups("<<lookups<<") : "<<lookups_time<<" us"<<endl;
    cout<<"Avg time taken for lookup operation "<<lookups_time/lookups<<" us"<<endl;

    return 0;
}