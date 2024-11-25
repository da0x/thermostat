//
// This file is part of gctrl.
//
// Copyright (C) 2024 Daher Alfawares
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
// Contact Information: www.gctrl.org
//

#include <cmath>
#include <numbers>
#include <iostream>
#include <machine/thermostat.hpp>
#include <chrono>
#include <thread>

int main()
{
    machine::thermostat thermostat;

    thermostat.machine_driver.subscribe();

    auto last_1hz = std::chrono::steady_clock::now();
    auto last_60hz = std::chrono::steady_clock::now();
    auto last_100hz = std::chrono::steady_clock::now();
    auto last_1khz = std::chrono::steady_clock::now();
    auto last_10khz = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();

        // 1 Hz (every 1 second)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_1hz).count() >= 1000) {
            thermostat.at_1hz();
            last_1hz = now;
        }

        // 60 Hz (every ~16.67 ms)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_60hz).count() >= 16) {
            thermostat.machine_driver.send();
            last_60hz = now;
        }

        // 100 Hz (every 10 ms)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_100hz).count() >= 10) {
            thermostat.at_100hz();
            last_100hz = now;
        }

        // 1 kHz (every 1 ms)
        if (std::chrono::duration_cast<std::chrono::microseconds>(now - last_1khz).count() >= 1000) {
            thermostat.at_1khz();
            last_1khz = now;
        }

        // 10 kHz (every 0.1 ms or 100 microseconds)
        if (std::chrono::duration_cast<std::chrono::microseconds>(now - last_10khz).count() >= 100) {
            thermostat.at_10khz();
            last_10khz = now;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
