#ifndef ARC_UTILITIES_TIMING_HPP
#define ARC_UTILITIES_TIMING_HPP

#include <chrono>
#include <string>
#include <map>
#include <vector>



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



    class Profiler
    {
    public:
        static Profiler* getInstance();

        static void initialize(size_t num_names, size_t num_events);

        static void addData(std::string name, double datum);

        static void startTimer(std::string timer_name);

        static double record(std::string timer_name);

        static std::vector<double> getData(std::string name);

        static void printSummary(std::string name);
                
        
    protected:
        std::map<std::string, std::vector<double> > data;
        std::map<std::string, Stopwatch> timers;
        std::vector<std::vector<double>> prealloc_buffer;
        
    private:
        static Profiler* m_instance;
        
    };





    /**
     *   Profiling Macros
     */

#ifdef ENABLE_PERFORMANCE_MONITORING
#define PROF_INITIALIZE(prealloc_num_names, prealloc_num_events)        \
    ::arc_utilities::Profiler::initialize(prealloc_num_names, prealloc_num_events);

#define PROF_START(name) \
    ::arc_utilities::Profiler::startTimer(name)

#define PROF_RECORD(name)                        \
    ::arc_utilities::Profiler::record(name)

#define PROF_PRINT_SUMMARY(name)
    ::arc_utilities::Profiler::printSummary(name)

    
#else
#define PROF_INITIALIZE(prealloc_num_names, prealloc_num_events) (void) 0
#define PROF_START(name) (void) 0
#define PROF_RECORD(name) (void) 0


#endif

    
    
}

#endif // ARC_UTILITIES_TIMING_HPP
