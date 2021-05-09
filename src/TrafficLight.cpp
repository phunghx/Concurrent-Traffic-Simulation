#include <iostream>
#include <random>
#include <thread>

#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_messages.empty(); }); 

    
    T msg = std::move(_messages.back());
    _messages.pop_back();

    return msg; 

}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    
        std::lock_guard<std::mutex> uLock(_mutex);

        
        _messages.push_back(std::move(msg));
        _cond.notify_one();

}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    
    while (true)  {
        if (this->light.receive()== TrafficLightPhase::green) {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    
    return _currentPhase;
}

void TrafficLight::simulate()
{
    
    this->threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(4, 6);
    bool flag = false;
    int duration;
    duration = distr(eng);
    while (true)
    {
        
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        
        if (flag)   {
             this->light.send(TrafficLightPhase::green);
             this->_currentPhase = TrafficLightPhase::green;
        }
        else   {
             this->light.send(TrafficLightPhase::red);  
             this->_currentPhase = TrafficLightPhase::red;
        }  
        flag = !flag;
        std::this_thread::sleep_for(std::chrono::seconds(duration));
    }
    
}

