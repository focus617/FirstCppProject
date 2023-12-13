#include "log.h"
#include "xuzy_math.h"

using std::cout;
using std::endl;
using xuzy::Log;

int main(){
    Log log;

    log.setLevel(Log::Level::LevelInfo);

    int result = xuzy::add(12,56);
    cout << "12 + 56 = " << result << endl;
    log.info("success");

    return EXIT_SUCCESS;
}