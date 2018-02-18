#include "message_logger.h"
#include "message_dest_dated_file.h"

int main(int ac, char **av)
{
    messageLogger log ;

    std::string basedir(".") ;
    std::shared_ptr<messageLoggerDest> dest_p = std::make_shared<messageDestDatedFile>(basedir) ;
    log.addDestination(dest_p) ;

    log.enableType(messageLogger::messageType::debug) ;
    log.enableSubsystem(1) ;

    log.startMessage(messageLogger::messageType::debug, 1) ;
    log << "Hello World" ;
    log.endMessage() ;
}
