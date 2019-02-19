#include <pthread.h>
#include <string>
#include <mutex>
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;
std::mutex mtx;

//#define NUM 1000        // 1k
//#define NUM 10000       // 10k
//#define NUM 100000      // 100k
//#define NUM 1000000     // 1m
//#define NUM 10000000    // 10m

#define NUM 10000000    // 10m
#define USE_LOCK

int index=0;
bool isThread1End=false;
bool isThread2End=false;
struct Data {
    string who;
    int count;
    int thread1_localIndex;
    int thread2_localIndex;
};

Data datas[NUM];

void counter1() {
    pthread_setname_np(pthread_self(), "thread1");
    
    while(index<NUM) {
        #ifdef USE_LOCK
        mtx.lock();
        #endif
        int local_index = index++;
        #ifdef USE_LOCK
        mtx.unlock();
        #endif
        
        datas[local_index].who = "Thread1";
        datas[local_index].count = local_index;
        datas[local_index].thread1_localIndex = local_index;
        //sleep(1);
    }
    isThread1End = true;
}

void counter2() {
    pthread_setname_np(pthread_self(), "Thread2");
    while(index<NUM) {
        #ifdef USE_LOCK
        mtx.lock();
        #endif
        int local_index = index++;
        #ifdef USE_LOCK
        mtx.unlock();
        #endif
        
        datas[local_index].who = "thread2";
        datas[local_index].count = local_index;
        datas[local_index].thread2_localIndex = local_index;
        //sleep(1);
    }
    isThread2End = true;
}

int main() {
    int cond1 = 0 ;
    
    thread t1(counter1);
    thread t2(counter2);
    t1.detach();
    t2.detach();
    // keepGoing=1;
    // while(keepGoing) {
    //     int key;
    //     cout<<"enter key: ";
    //     cin>>key;
    //     switch(key) {
    //         case 1: {
    //             keepGoing=0;
    //             cout<<"Exit--"<<endl;
    //             break;
    //         }
    //         case 2: {
    //             cout<<"Counter: "<<count<<endl;
    //             break;
    //         }
    //         default: {
    //             cout<<"Invalid input"<<endl;
    //             continue;
    //         }
    //     }
    // }
    // for(int i=0; i< 4000; i++){
    //       printf("counter : %d\n",count);
    //       sleep(1);
    // }
    while(!isThread1End) {
        
    }
    while(!isThread2End) {
        
    }
    cout<<"Start"<<endl;
    for(int i=0; i<NUM; i++) {
        //cout<<datas[i].who<<": "<<datas[i].count<<endl;
        cout<<datas[i].who<<": "<<datas[i].count;
        cout<<": "<<datas[i].thread1_localIndex<<": "<<datas[i].thread2_localIndex<<endl;
    }
    cout<<"End"<<endl;
    return 0;
}