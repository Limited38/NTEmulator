#pragma once
#ifndef __LOG_H
#define __LOG_H
#include <pantheios/pantheios.hpp>  
#include <pantheios/backends/bec.file.h>
#include <pantheios/inserters/integer.hpp>
#include <pantheios/inserters/blob.hpp>
#include <pantheios/inserters/boolean.hpp>
#include <pantheios/inserters/pointer.hpp>
#include <pantheios/inserters/fmt.hpp>
#include <pantheios/inserters/character.hpp>

#define panthstr(x) PANTHEIOS_LITERAL_STRING(x)

// pantheios set process identification
PANTHEIOS_EXTERN_C const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = panthstr("nstory_login");

namespace panth = pantheios;

//pantheios::log_DEBUG(PSTR("debug statement"));
//pantheios::log_INFORMATIONAL(PSTR("informational statement"));
//pantheios::log_NOTICE(PSTR("notice statement"));
//pantheios::log_WARNING(PSTR("warning statement"));
//pantheios::log_ERROR(PSTR("error statement"));
//pantheios::log_CRITICAL(PSTR("critical statement"));
//pantheios::log_ALERT(PSTR("alert statement"));
//pantheios::log_EMERGENCY(PSTR("emergency statement"));

#endif