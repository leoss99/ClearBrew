# Load Cell

In this project, we used:

- TAL220B load cell - 5kg max

- Hx711 load cell amplifier


In this directory, you will find an excel notebook.
This spreadsheet contains the data used to calibrate the load cell.
We measured multiple weights using an off-the-shelf kitchen scale,
then placed these weights on our load cell and recorded the values
read from the load cell amplifier.
By plotting the data, we say that the relationship between signal and weight is
linear, so we simply converted the signal to a weight using the calculated
slope and intercept.