//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-09 20:12:57
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
// Base code for Globe users
//---------------------------------------------------------------------------
// user of this library can define its own exception codes by using the
// following value as a base code. for example :
//   #define MY_FIRST_EXCODE   (::glb::EXCODE_USER_BASE + 1)
//   #define MY_SECOND_EXCODE  (::glb::EXCODE_USER_BASE + 2)
EXCODE_DEFINE(5000, EXCODE_USER_BASE, "-")


//---------------------------------------------------------------------------
// Standard exception codes
//---------------------------------------------------------------------------
// global codes
EXCODE_DEFINE(   1, EXCODE_NONE,          "No given exception code")
EXCODE_DEFINE(   2, EXCODE_UNKNOWN,       "Unknown error")
EXCODE_DEFINE(   3, EXCODE_STD_EXCEPTION, "Standard exception") // std::exception
EXCODE_DEFINE(   4, EXCODE_ASSERT,        "Assertion")
EXCODE_DEFINE(   5, EXCODE_SYNTAX,        "Invalid syntax")
EXCODE_DEFINE(   6, EXCODE_PARAM,         "Invalid parameter(s)")
EXCODE_DEFINE(   7, EXCODE_MEMORY,        "Out of memory")
EXCODE_DEFINE(   8, EXCODE_OPERATION,     "Invalid/unsupported operation")

// generic exceptions
EXCODE_DEFINE( 100, EXCODE_SYSTEM_API, "Standard/System API call failed")

// physical filesystem
EXCODE_DEFINE( 200, EXCODE_FILE_DOESNTEXIST, "File does not exist")
EXCODE_DEFINE( 201, EXCODE_FILE_OPEN,        "Unable to open file")
EXCODE_DEFINE( 202, EXCODE_FILE_CREATE,      "Unable to create file")
EXCODE_DEFINE( 203, EXCODE_FILE_NOTREADABLE, "File is not readable")
EXCODE_DEFINE( 204, EXCODE_FILE_NOTWRITABLE, "File is not writable")
EXCODE_DEFINE( 205, EXCODE_FILE_READING,     "Error while reading file")
EXCODE_DEFINE( 206, EXCODE_FILE_WRITING,     "Error while writing to file")
EXCODE_DEFINE( 207, EXCODE_FILE_SEEKING,     "Error while seeking into file")
EXCODE_DEFINE( 208, EXCODE_FILE_DISKFULL,    "Error while writing to file (maybe disk is full)")
EXCODE_DEFINE( 209, EXCODE_FILE_MAPPING,     "Error while mapping file into memory")
EXCODE_DEFINE( 210, EXCODE_FILE_EOF,         "Unexpected end of file")
