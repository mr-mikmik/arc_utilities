#include <fstream>
#include <iomanip>
#include <string>

#ifndef LOG_HPP
#define LOG_HPP

#define LOG( log, message )                            \
    (log).logMessage(                                  \
        static_cast<std::ostringstream&>(              \
            std::ostringstream().flush() << (message)  \
        ).str()                                        \
    )

#define LOG_COND( log, cond, message )                 \
    if ( cond ) LOG( log, message )

namespace Log
{
    class Log
    {
        public:
            Log( const std::string& filename )
                : filename_( filename )
                , out_file_( filename, std::ios_base::out | std::ios_base::trunc )
            {
                time_t rawtime;
                tm * timeinfo;
                char buffer[80];

                time(&rawtime);
                timeinfo = localtime(&rawtime);

                strftime( buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo );

                out_file_ << buffer << std::endl;
            }

            /** Copy constructor */
            Log( const Log& other )
                : filename_( other.filename_ )
                , out_file_( filename_, std::ios_base::out | std::ios_base::app )
            {
            }

            /** Move constructor */
            Log( Log&& other )
                : filename_( other.filename_ )
                , out_file_( filename_, std::ios_base::out | std::ios_base::app )
            {
                other.out_file_.close();
            }

            /** Destructor */
            ~Log()
            {
                if ( out_file_.is_open() )
                {
                    out_file_.close();
                }
            }

            /** Copy assignment operator */
            Log& operator= ( const Log& other )
            {
                Log tmp( other ); // re-use copy-constructor
                *this = std::move( tmp ); // re-use move-assignment
                return *this;
            }

            /** Move assignment operator */
            Log& operator= ( Log&& other)
            {
                std::swap( filename_, other.filename_ );
                other.out_file_.close();

                if ( out_file_.is_open() )
                {
                    out_file_.close();
                }

                out_file_.open( filename_, std::ios_base::out | std::ios_base::app );

                return *this;
            }

            void logMessage( const std::string& message )
            {
                out_file_ << message << std::endl;
            }

        private:
            std::string filename_;
            std::ofstream out_file_;
    };
}

#endif // LOG_HPP