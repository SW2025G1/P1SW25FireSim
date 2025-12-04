# Wildfire Simulator: C Simplified Rothermel Cellular Automata
The purpose of this project is to make available a wildfire simulation program, which demonstrates the effects of the input parameters (fuels, topography, wind speed) on the behavior of fire simulated. This program is not an accurate fire simulator, albeit a learning tool which demonstrates the fire dynamics calculations in a simplified, Rothermel adapted design. The approach is simplifying the Rothermel model to its components: the isotropic (Pre-Full-Rothermel-simulated base rate of spread rates for the fuel models) with the anisotropic components (wind and slope factors adapted with adjustable fuel specific sensitivity coefficients). The result is a program which approximates the Rothermel Cellular Automata approach in a simpler, understandable manner.

The initial inputs are:
- Input file path (synthetic homogenous landscapes are made available to demonstrate fuel models and slopes scenarios)
- Wind speed and Wind direction setting
- Moisture of dead fuels (the long term moisture scenario)

The program then runs a simulation loop which asks the user for time to be simulated. A html file is generated and opened, which shows a color grid map of the current cells statuses.
- Green = Cell with burnable fuel not yet ignited
- Yellow = Cell which is progressing toward being ignited
- Red = Cell which is ignited

This program was developed as the product of a Software Engineering Semester 1 project.