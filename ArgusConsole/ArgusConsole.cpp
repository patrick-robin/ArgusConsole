// ArgusConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "argus_monitor_data_accessor.h"
#include "argus_monitor_data_api.h"

int main()
{
    argus_monitor::data_api::ArgusMonitorDataAccessor data_accessor_{};

    bool       callback_called{ false };

    auto const on_temperature_changed
        = [&callback_called](argus_monitor::data_api::ArgusMontorData const& new_sensor_data) {

        // DO ALL THE PROCESSING HERE, ALL DATA IS IN new_sensor_data
        callback_called = true;

        std::cout << "something\n";
    };

    data_accessor_.RegisterSensorCallbackOnDataChanged(on_temperature_changed);
    data_accessor_.Open();
    bool isOpen = data_accessor_.IsOpen();

    std::cout << isOpen;
    std::cout << "\n";

    while (callback_called == false)
    {
        std::cout << "Waiting\n";
    }

    data_accessor_.Close();
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
