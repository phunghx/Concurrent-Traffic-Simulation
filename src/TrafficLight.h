#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;




template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _messages;

};
enum TrafficLightPhase {
   red,
   green
};



class TrafficLight : TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
    

    // getters / setters

    // typical behaviour methods

private:
    

    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> light;

    std::condition_variable _condition;
    std::mutex _mutex;
    void cycleThroughPhases();
};

#endif