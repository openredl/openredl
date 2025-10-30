# Basic Skill Tests
## Test 1: Firmware Basic Configuration
0. Select a differential channel to work with (e.g. DIFF1).
1. Measure the open circuit voltage of a solar cell.
2. Select the adequate gain/range and set it.
3. Give the required storage interval.
4. Log the following parameters: (min/max/mean)
5. Give the logged parameters a name (e.g. Voc_mean)

## Test 2: Calibration Values and Labels(Continuation from Test 1)
7. Measure the solar cell short circuit current with the help of a shunt resistance.
8. Select the adequate gain/range.
9. Insert the shunt resistance multiplication factor to convert V to I (pay attention to the units mV and V).
10. Log the following parameters: (min/max/mean)
11. Update the log label name to Isc.

## Test 3: Test PT100(0) Temperature Sensors (In other differential channel e.g. DIFF2)
13. Identify which sensor you have (PT100 or PT1000) measuring its resistance.
14. Connect to the 1mA constant current source and the data logger.
15. Select the adequate gain/range.
16. Select the relevant parameters to log.
17. Update the log label name to PT100 or PT1000.

## Test 3a: Test Thermacouple Temperature Sensors (In other differential channel, e.g. DIFF2)
13. Identify which sensor you.
14. Set an estimated offset (e.g. 20°C) and multiplication factor (e.g. 43uV/C)
15. Select the adequate gain/range.
16. Select the relevant parameters to log.
17. Update the log label name to Therm

## Test 4: Plotting with the real time plot
18. Create a copy of the realtimeplot.py
19. Configure the script to plot your differential channels.
Create one plot for each sensor.
19. Test it and test other configuration parameters.
