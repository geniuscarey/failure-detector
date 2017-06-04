#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main( int argc, char* argv[] )
{
    el::Configurations conf("../log.conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);

    Catch::Session session; // There must be exactly one instance

    // writing to session.configData() here sets defaults
    // this is the preferred way to set them

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 ) // Indicates a command line error
        return returnCode;

    // writing to session.configData() or session.Config() here 
    // overrides command line args
    // only do this if you know you need to

    int numFailed = session.run();
    // Note that on unices only the lower 8 bits are usually used, clamping
    // the return value to 255 prevents false negative when some multiple
    // of 256 tests has failed
    return ( numFailed < 0xff ? numFailed : 0xff );
}
