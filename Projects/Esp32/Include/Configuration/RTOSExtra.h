#pragma once

#include "freertos/FreeRTOS.h"

#define configWIFISVC_STACK_DEPTH (1024 * 3)
#define configFWUPDATESVC_STACK_DEPTH (1024 * 4)
#define configMENUSVC_STACK_DEPTH (1024 * 4)
#define configINPUTSCANNERSVC_STACK_DEPTH (1024 * 2)
#define configLASERSVC_STACK_DEPTH (1024 * 4)

#define configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED (configLASERSVC_STACK_DEPTH          + \
                                                configMENUSVC_STACK_DEPTH            + \
                                                configINPUTSCANNERSVC_STACK_DEPTH)
