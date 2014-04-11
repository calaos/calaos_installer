/******************************************************************************
 **  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos Home.
 **
 **  Calaos Home is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Calaos Home is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
//-----------------------------------------------------------------------------
#ifndef CUTILS_H
#define CUTILS_H
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <stack>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <Params.h>
#include <base64.h>
#include <QtCore>

//-----------------------------------------------------------------------------
using namespace std;

//Here are some specific win32 code.
#ifdef _WIN32
#include <unistd.h>
char *realpath(const char *file_name, char *resolved_name);
typedef unsigned int uint;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

//-----------------------------------------------------------------------------
// Some common defines
//-----------------------------------------------------------------------------
#define DEFAULT_CONFIG_PATH     "/mnt/ext3/calaos/"
#define DEFAULT_THEME           "/usr/share/calaos/themes/default.edj"
#define DEFAULT_BG              "/usr/share/calaos/themes/default.edj"
#define DEFAULT_URL             "http://update.calaos.fr/fwupdate.xml"
#define DEFAULT_CONFIG          "/mnt/ext3/calaos/local_config.xml"
#define WIDGET_CONFIG           "/mnt/ext3/calaos/widgets.xml"
#define ZONETAB                 "/usr/share/zoneinfo/zone.tab"
#define CURRENT_ZONE            "/etc/timezone"
#define LOCALTIME               "/etc/localtime"
#define ZONEPATH                "/usr/share/zoneinfo/"

#define CALAOS_NETWORK_URL      "https://www.calaos.fr/calaos_network"

// The size of the window. For now The Calaos touchscreen gui is only designed
// to fit a screen of 1024x768 pixels.
#define WIDTH   1024
#define HEIGHT   768
//-----------------------------------------------------------------------------
#define PI 3.14159265358979323846
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define RED "\x1b[31;01m"
#define DARKRED "\x1b[31;06m"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32;06m"
#define YELLOW "\x1b[33;06m"

#define WAGO_HOST       "10.0.0.123"

#ifndef WAGO_LISTEN_PORT
#define WAGO_LISTEN_PORT        4646
#endif
#ifndef TCP_LISTEN_PORT
#define TCP_LISTEN_PORT         4456
#endif

//Sleep time for SocketManager thread
#ifndef THREAD_SLEEP
#define THREAD_SLEEP            60 / 2
#endif

//IR Timeout when learning new codes
#ifndef IR_TIMEOUT
#define IR_TIMEOUT              30 * 1000
#endif

#ifndef BCAST_UDP_PORT
#define BCAST_UDP_PORT          4545
#endif

#define NETWORK_TIMEOUT         30000

#define SPECIAL_ROOM_TYPE               "Internal"
#define SPECIAL_ROOM_SIMPLESCENARIO     "SimpleScenario"
#define SPECIAL_RULES_SIMPLESCENARIO    "SimpleScenarioRules"

#define WAGO_KNX_START_ADDRESS          6144
#define WAGO_841_START_ADDRESS          4096

/* These macros are usefull to delete/free/... an object and set it to NULL */
#define DELETE_NULL(p) \
    if (p) { delete p; p = NULL; }

#define FREE_NULL(p) \
    if (p) { free(p); p = NULL; }

#define DELETE_NULL_FUNC(fn, p) \
    if (p) { fn(p); p = NULL; }

#define cDebug qDebug
#define cInfo qDebug
#define cWarning qDebug
#define cError qDebug
#define cCritical qDebug

//Curl callback
int CURL_write_callback(void *buffer, size_t size, size_t nmemb, void *stream);
int CURL_writebuf_callback(void *buffer, size_t size, size_t nmemb, void *stream);
//-----------------------------------------------------------------------------
namespace Utils
{
bool file_copy(std::string source, std::string dest);

string url_encode(string str);
string url_decode(string str);
std::string url_decode2(std::string str); //decode 2 times
int htoi(char *s);
string time2string(long s, long ms = 0);
string time2string_digit(long s, long ms = 0);

/* usefull string utilities */
void split(const string &str, vector<string> &tokens, const string &delimiters = " ", int max = 0);
void remove_tag(string &source, const string begin_tag, const string end_tag);
void replace_str(string &source, const string searchstr, const string replacestr);
void trim_right(std::string &source, const std::string &t);
void trim_left(std::string &source, const std::string &t);

//Return a value rounded to 2 decimal after the dot
double roundValue(double value);

//Parse a result string into an array of Params.
void parseParamsItemList(string l, vector<Params> &res, int start_at = 0);

//!decode a BASE64 string
std::string Base64_decode(std::string &str);
void *Base64_decode_data(std::string &str);
//!encode a BASE64 string
std::string Base64_encode(std::string &str);
std::string Base64_encode(void *data, int size);

//-----------------------------------------------------------------------------
template<typename T>
bool is_of_type(const std::string &str)
{
    std::istringstream iss(str);
    T tmp;
    iss >> tmp;
    return iss.eof();
}
template<typename T>
bool from_string(const std::string &str, T &dest)
{
    std::istringstream iss(str);
    iss >> dest;
    return iss.eof();
}
template<typename T>
std::string to_string( const T & Value )
{
    std::ostringstream oss;
    oss << Value;
    return oss.str();
}

//Some usefull fonctors
struct UrlDecode
{
    template <class T> void operator ()(T &str) const
    {
        str = Utils::url_decode2(str);
    }
};
struct Delete
{
    template <class T> void operator ()(T *&p) const
    {
        DELETE_NULL(p)
    }
};
class to_lower
{
public:
    char operator() (char c) const
    {
        return tolower(c);
    }
};
class to_upper
{
public:
    char operator() (char c) const
    {
        return toupper(c);
    }
};

class DeletorBase
{
public:
    virtual ~DeletorBase() {}
    virtual void operator() (void *b) const
    {
        (void)b;
        cerr << "DeletorBase() called, this is an error. It should never happen"
             << ", because it means the application leaks memory!" << endl;
    }
};

//Fonctor to delete a void * with a specified type
template<typename T>
class DeletorT: public DeletorBase
{
public:
    virtual void operator() (void *b) const
    {
        T base = reinterpret_cast<T>(b);
        if (base) delete base;
    }
};
//-----------------------------------------------------------------------------
//Used by the CURL callback
typedef struct file_curl
{
    char *fname;
    FILE *fp;
} File_CURL;
typedef struct buffer_curl
{
    void *buffer;
    unsigned int bufsize;
} Buffer_CURL;
//-----------------------------------------------------------------------------
class line_exception : public std::exception
{
private:
    std::string msg;

public:
    line_exception( const char * Msg, int Line )
    {
        std::ostringstream oss;
        oss << "Error line " << Line << " : " << Msg;
        msg = oss.str();
    }

    virtual ~line_exception() throw()
    { }

    virtual const char * what() const throw()
    {
        return msg.c_str();
    }
};


//-----------------------------------------------------------------------------
typedef enum { TBOOL, TINT, TSTRING, TUNKNOWN } DATA_TYPE;
//-----------------------------------------------------------------------------
typedef unsigned short UWord;
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif
