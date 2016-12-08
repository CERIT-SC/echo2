//
//  GlobalSettings.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 12/1/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef EchoErrorCorrection_GlobalSettings_h
#define EchoErrorCorrection_GlobalSettings_h

#define APPLICATION_NAME_STRING     "Echo Error Correction"
#define APPLICATION_VERSION_STRING  "2.1.0 (build)"

#define RETURN_SUCCESS        0
#define RETURN_INC_OPT        1
#define RETURN_FILES_SKIPPED  2
#define RETURN_LOG_FILE_ERROR 3
#define RETURN_MEMORY_ERROR   4
#define RETURN_UNKNOWN_ERROR  5
#define RETURN_COMPATIBILITY_ERROR 6

#define HASH_TABLE_SIZE_EXP_MAX 32
#define HASH_TABLE_SIZE_EXP_MIN 20

#define OUTPUT_FILE_NAME_EXTENSION "-corrected"

#endif
