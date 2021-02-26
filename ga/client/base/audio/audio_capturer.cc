//
// Aspia Project
// Copyright (C) 2020 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include "base/audio/audio_capturer.h"

#include "proto/desktop.pb.h"

namespace base {

// Returns true if the sampling rate is supported by Pepper.
bool AudioCapturer::isValidSampleRate(int sample_rate)
{
    switch (sample_rate)
    {
        case proto::AudioPacket::SAMPLING_RATE_44100:
        case proto::AudioPacket::SAMPLING_RATE_48000:
        case proto::AudioPacket::SAMPLING_RATE_96000:
        case proto::AudioPacket::SAMPLING_RATE_192000:
            return true;

        default:
            return false;
    }
}

} // namespace base
