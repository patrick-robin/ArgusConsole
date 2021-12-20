// ArgusConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include "argus_monitor_data_accessor.h"
#include "argus_monitor_data_api.h"

using namespace std::chrono_literals;

void PrintSensorData(argus_monitor::data_api::ArgusMonitorData const& sensor_data, std::size_t const sensor_index)
{
    auto const sensor_data_offset = sensor_data.OffsetForSensorType[sensor_index];
    auto const sensor_data_count = sensor_data.SensorCount[sensor_index];

    for (std::size_t index{}; index < sensor_data_count; ++index) {
        std::wcout << sensor_data.SensorData[sensor_data_offset + index].Label << L": " << sensor_data.SensorData[sensor_data_offset + index].Value
            << L' ' << sensor_data.SensorData[sensor_data_offset + index].UnitString << L'\n';
    }
}

int main()
{
    std::future<void> wait_for_key = std::async([]() { std::cin.get(); });

    auto const new_sensor_data_available = [](argus_monitor::data_api::ArgusMonitorData const& new_sensor_data) {
        std::cout << "New Argus Monitor sensor data available:\n\n";
        std::cout << "Argus Monitor Version: " << std::to_string(new_sensor_data.ArgusMajor) << "." << std::to_string(new_sensor_data.ArgusMinorA)
            << "." << std::to_string(new_sensor_data.ArgusMinorB) << "." << std::to_string(new_sensor_data.ArgusBuild) << '\n';
        std::cout << "Total number of sensor values:" << std::to_string(new_sensor_data.TotalSensorCount) << '\n';

        // Example: print all CPU temperature values
        PrintSensorData(new_sensor_data, argus_monitor::data_api::SENSOR_TYPE_CPU_TEMPERATURE);

        // and some extra values (e.g. CCDx temps for AMD processors)
        PrintSensorData(new_sensor_data, argus_monitor::data_api::SENSOR_TYPE_CPU_TEMPERATURE_ADDITIONAL);

        // Example2: print all SSD/HDD temperatures
        PrintSensorData(new_sensor_data, argus_monitor::data_api::SENSOR_TYPE_DISK_TEMPERATURE);

        // Example3: print all GPU temperatures
        PrintSensorData(new_sensor_data, argus_monitor::data_api::SENSOR_TYPE_GPU_TEMPERATURE);


        std::cout << "\n\nWaiting for new sensor data -- press RETURN or Ctrl-C to exit.\n";
    };


    argus_monitor::data_api::ArgusMonitorDataAccessor data_accessor_{};
    data_accessor_.RegisterSensorCallbackOnDataChanged(new_sensor_data_available);
    while (!data_accessor_.Open()) {
        std::cout << "Waiting for Argus Monitor API connection to become available\n";
        if (wait_for_key.wait_for(1s) == std::future_status::ready) {
            data_accessor_.Close();
            return -1;
        }
    }

    std::cout << "Connection to Argus Monitor established\n";
    std::cout << "Waiting for new sensor data -- press RETURN or Ctrl-C to exit.\n";

    while (wait_for_key.wait_for(1s) != std::future_status::ready) {
        ;
    }

    data_accessor_.Close();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
