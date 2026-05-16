.. zephyr:code-sample:: hello_cpp_world
   :name: Hello C++ world

   Print "Hello World", Custom Board name, pre-booting message and Soc info to the console in C++.

Overview
********

A simple :ref:`C++ <language_cpp>` sample that can be used with many supported board and prints
"Hello, C++ world!" along with board and SoC info to the console.

Building and Running
********************
west build -b my_board_scratch/nrf52840 hello_world_scratch -p 

west flash

Sample Output
=============

.. Serial Monitor console on COM7 (usually for arduino nano 33ble sense nRF52840 MCU): console

---- Opened the serial port COM7 ----
   -----------Booting....Before main loop begins. Test Message--------------
   Board my_board_scratch with SoC:nrf52840 from scratch initialised

   -----------End of Test Message and now entering main loop----------------*** Booting nRF Connect SDK v3.2.4-4c3fc0d44534 ***
   *** Using Zephyr OS v4.2.99-9673eec75908 ***
   --------------- Inside main application loop-----------------------
   Hello, C++ world! Board: my_board_scratch       , SoC: nrf52840

   ---------------End of main application loop--------------
---- Closed the serial port COM7 ----
