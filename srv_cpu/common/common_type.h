#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#include <memory>
#include "spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace sc_core;

enum msg_type {
    CTRL_MSG = 0,
    DATA_MSG = 1,
    MAX_MSG
};

typedef struct ctrl_msg
{
    msg_type type;
    int      credit;
} MY_CTRL_T;

typedef struct data
{
    int a;
    int b;
    int c;
} MY_DAT_T;

typedef struct api
{
    ctrl_msg msg;
    std::shared_ptr<void> dat;
} MY_API_T;

#endif