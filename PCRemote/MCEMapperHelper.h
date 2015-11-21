/*
 * MCEMapperHelper.h
 *
 * Created: 17.11.2015 11:46:56
 *  Author: Bernd
 */ 


#ifndef MCEMAPPERHELPER_H_
#define MCEMAPPERHELPER_H_
#include "conf.h"
#include "MCEMapper.h"

#define SEND_CODE(code, mk_or_bk) { \
    switch (mk_or_bk) \
    { \
        case MAKE: \
        SEND_MAKE(code); \
        return; \
        case BREAK: \
        SEND_BREAK(code); \
        return;\
    } \
}

#define SEND_EXT_CODE(code, mk_or_bk) { \
    switch (mk_or_bk) \
    { \
        case MAKE: \
        SEND_EXT_MAKE(code); \
        return; \
        case BREAK: \
        SEND_EXT_BREAK(code); \
        return;\
    } \
}

#define SEND_2KEY_CODE(code1, isext1, code2, isext2, mk_or_bk) { \
    switch (mk_or_bk) \
    { \
        case MAKE: \
            if (isext1) \
            { \
                SEND_EXT_MAKE(code1) \
            } \
            else \
            { \
                SEND_MAKE(code1); \
            } \
            if (isext2) \
            { \
                SEND_EXT_MAKE(code2); \
            } \
            else \
            { \
                SEND_MAKE(code2); \
            } \
            return; \
        case BREAK: \
            if (isext2) \
            { \
                SEND_EXT_BREAK(code2); \
            } \
            else \
            { \
                SEND_BREAK(code2); \
            } \
            if (isext1) \
            { \
                SEND_EXT_BREAK(code1) \
            } \
            else \
            { \
                SEND_BREAK(code1); \
            } \
            return; \
    } \
}

#endif /* MCEMAPPERHELPER_H_ */