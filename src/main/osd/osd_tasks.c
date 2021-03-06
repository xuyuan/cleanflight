/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform.h>

#include "osd/osd_tasks.h"

#include "scheduler/scheduler.h"

#include "cleanflight_osd.h"

#define TASK_QUEUE_ARRAY_SIZE (TASK_COUNT + 1) // extra item for NULL pointer at end of queue

const uint32_t taskQueueArraySize = TASK_QUEUE_ARRAY_SIZE;
const uint32_t taskCount = TASK_COUNT;
cfTask_t* taskQueueArray[TASK_QUEUE_ARRAY_SIZE];

cfTask_t cfTasks[] = {
    [TASK_SYSTEM] = {
        .taskName = "SYSTEM",
        .taskFunc = taskSystem,
        .desiredPeriod = TASK_PERIOD_MS(100),
        .staticPriority = TASK_PRIORITY_HIGH,
    },

    [TASK_CYCLE_TIME] = {
        .taskName = "CYCLE_TIME",
        .taskFunc = taskUpdateCycleTime,
        .desiredPeriod = TASK_PERIOD_HZ(1000),
        .staticPriority = TASK_PRIORITY_LOW,
    },

    [TASK_MSP_SERVER] = {
        .taskName = "MSP_SERVER",
        .taskFunc = taskMSP,
        .desiredPeriod = TASK_PERIOD_HZ(100),     // 100 Hz should be enough to flush up to 115 bytes @ 115200 baud
        .staticPriority = TASK_PRIORITY_MEDIUM,
    },

    [TASK_STATUS_LED] = {
        .taskName = "STATUS_LED",
        .taskFunc = taskStatusLed,
        .desiredPeriod = TASK_PERIOD_HZ(100),
        .staticPriority = TASK_PRIORITY_LOW,
    },

    [TASK_HARDWARE_WATCHDOG] = {
        .taskName = "HW_WATCHDOG",
        .taskFunc = taskHardwareWatchdog,
        .desiredPeriod = TASK_PERIOD_HZ(1),
        .staticPriority = TASK_PRIORITY_MEDIUM,
    },

    [TASK_BATTERY] = {
        .taskName = "BATTERY",
        .taskFunc = taskUpdateBattery,
        .desiredPeriod = TASK_PERIOD_HZ(30),
        .staticPriority = TASK_PRIORITY_MEDIUM,
    },

    [TASK_DRAW_SCREEN] = {
        .taskName = "DRAW_SCREEN",
        .taskFunc = taskDrawScreen,
        .desiredPeriod = TASK_PERIOD_HZ(30),
        .staticPriority = TASK_PRIORITY_MEDIUM,
    },

    [TASK_UPDATE_FC_STATE] = {
        .taskName = "UPDATE_FC_STATE",
        .taskFunc = taskUpdateFCState,
        .desiredPeriod = TASK_PERIOD_HZ(30),
        .staticPriority = TASK_PRIORITY_MEDIUM,
    },

#ifdef TRANSPONDER
    [TASK_TRANSPONDER] = {
        .taskName = "TRANSPONDER",
        .taskFunc = taskTransponder,
        .desiredPeriod = TASK_PERIOD_MS(4),
        .staticPriority = TASK_PRIORITY_LOW,
    },
#endif

    [TASK_TEST] = {
        .taskName = "TEST",
        .taskFunc = taskTest,
        .desiredPeriod = TASK_PERIOD_HZ(1),
        .staticPriority = TASK_PRIORITY_LOW,
    },
};
