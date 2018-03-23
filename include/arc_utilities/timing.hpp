#ifndef ARC_UTILITIES_TIMING_HPP
#define ARC_UTILITIES_TIMING_HPP



/*
 *  timing.hpp includes utilities for recording of execution time
 *
 *  Stopwatch wraps chrono::steady_clock. A global stopwatch is provided.
 * 
 *  Profiler is a singleton which allows storage of many timers. 
 *  It is not indended that the Profiler class be used directly. 
 *  Instead use the provided macros.
 *  Profiling can be enabled/disabled using #define ENABLE_PROFILING
 * 
 *  example:
 * 
 *  #define ENABLE_PROFILING
 *  #include timing.hpp
 *  PROFILE_REINITIALIZE(10,100);
 *  ...
 *  PROFILE_START("foo do stuff");
 *  foo_do_stuff();
 *  PROFILE_RECORD("foo do stuff");
 *  ...
 *  PROFILE_PRINT_SUMMARY_FOR_SINGLE("foo do stuff");
 * 
 *
 */




#include <chrono>
#include <string>
#include <map>
#include <vector>


/**
 *   .======================.
 *  ||   Profiling Macros   ||
 *   '======================'
 */

/* to use profiling, "#defined ENABLE_PROFILING" needs to appear before "#import timing.hpp"*/
#ifdef ENABLE_PROFILING

/* Clears all stored data and preallocates space for later recordings*/
#define PROFILE_REINITIALIZE(prealloc_num_names, prealloc_num_events)        \
    ::arc_utilities::Profiler::reset_and_preallocate(prealloc_num_names, prealloc_num_events);

/* Clears the data for a single stored name */
#define PROFILE_RESET(name) \
    ::arc_utilities::Profiler::reset(name);

/* Starts (or restarts) the specified stopwatch */
#define PROFILE_START(name) \
    ::arc_utilities::Profiler::startTimer(name);

/* Record the value of the specified timer. Does not stop or restart the timer */
#define PROFILE_RECORD(name) \
    ::arc_utilities::Profiler::record(name);

/* Print a summary of the data for name */
#define PROFILE_PRINT_SUMMARY_FOR_SINGLE(name) \
    ::arc_utilities::Profiler::printSingleSummary(name);

/* Print a condensed summary for each of name in names */
#define PROFILE_PRINT_SUMMARY_FOR_GROUP(names) \
    ::arc_utilities::Profiler::printGroupSummary(names);
    
#else
/*Void macros make it easy to turn off profiling*/
#define PROFILE_REINITIALIZE(prealloc_num_names, prealloc_num_events) (void) 0
#define PROFILE_RESET(name) (void) 0
#define PROFILE_START(name) (void) 0
#define PROFILE_RECORD(name) (void) 0
#define PROFILE_PRINT_SUMMARY_FOR_SINGLE(name) (void) 0
#define PROFILE_PRINT_SUMMARY_FOR_GROUP(names) (void) 0



#endif





namespace arc_utilities
{
    enum StopwatchControl {RESET, READ};

    class Stopwatch
    {
        public:
            Stopwatch()
                : start_time_(std::chrono::steady_clock::now())
            {}

            double operator() (const StopwatchControl control = READ)
            {
                const auto end_time = std::chrono::steady_clock::now();
                if (control == RESET)
                {
                    start_time_ = end_time;
                }

                return std::chrono::duration<double>(end_time - start_time_).count();
            }

        private:
            std::chrono::steady_clock::time_point start_time_;
    };

    double GlobalStopwatch(const StopwatchControl control = READ);



    /* 
     *  Profiler: 
     *  Note - You should probably use the macros instead of calling the profiler directly
     */

    class Profiler
    {
    public:
        static Profiler* getInstance();

        /*
         *  Clears existing data and preallocates memory for data storage
         */
        static void reset_and_preallocate(size_t num_names, size_t num_events);

        static void reset(std::string name);

        static void addData(std::string name, double datum);

        static void startTimer(std::string timer_name);

        static double record(std::string timer_name);

        static std::vector<double> getData(std::string name);

        static void printSingleSummary(std::string name);
        
        static void printGroupSummary(const std::vector<std::string> &names);
                
        
    protected:
        std::map<std::string, std::vector<double> > data;
        std::map<std::string, Stopwatch> timers;
        std::vector<std::vector<double>> prealloc_buffer;
        
    private:
        static Profiler* m_instance;
        
    };






    
    
}

#endif // ARC_UTILITIES_TIMING_HPP
