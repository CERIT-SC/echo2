//
//  debug.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//

#ifndef EchoErrorCorrection_debug_h
#define EchoErrorCorrection_debug_h



#define __DEBUG__ //comment when done



//main testing directive
#ifdef __DEBUG__
    #define STANDARD_DEBUG
    #define SHORT_OUTPUT_DEB
    //#define LONG_OUTPUT_DEB
    #include <iostream>

#endif //__DEBUG__

#ifndef __DEBUG__
    #define NDEBUG
#endif
#include <cassert>




#endif
