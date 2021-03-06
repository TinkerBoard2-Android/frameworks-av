/*
*opyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "wfd_utils"
#include <utils/Log.h>
#include <media/stagefright/foundation/avc_utils.h>

#include "WfdUtils.h"

namespace android {

template <typename T>
bool IsIDRInternal(const sp<T> &buffer) {
    const uint8_t *data = buffer->data();
    size_t size = buffer->size();

    bool foundIDR = false;

    const uint8_t *nalStart;
    size_t nalSize;
    while (getNextNALUnit(&data, &size, &nalStart, &nalSize, true) == OK) {
        if (nalSize == 0u) {
            ALOGW("skipping empty nal unit from potentially malformed bitstream");
            continue;
        }

        unsigned nalType = nalStart[0] & 0x1f;

        if (nalType == 5) {
            foundIDR = true;
            break;
        }
    }

    return foundIDR;
}

bool IsIDR(const sp<ABuffer> &buffer) {
    return IsIDRInternal(buffer);
}

bool IsIDR(const sp<MediaCodecBuffer> &buffer) {
    return IsIDRInternal(buffer);
}

}  // namespace android
