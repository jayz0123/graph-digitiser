# Graph Digitiser 
The basic idea of the algorithm of the 1D graphs digitiser is to open an image file, then let the user click on certain points to calculate the calibration factor based on the input values that the user has been input. Once the programme complete the calibration, it allows the user to click on the graph and transfer the pixel coordinates to the real-world coordinates based on the calibration.

## How it works
- The first step is to open an image file in order to calibrate prior to measuring the graph. The programme loads the image file and stores it for any further operation. The programme will automatically look for .jpg and .png images at first.

- The next step is to let the user calibrate on the image. The progarmme calculates the calibration factor based on the four clicked pixels on the graph and the real-world values that the user has input.

- When completing the calibration, the programme uses the calibration factor to calculate the real-world coordinate of any pixel that the user clicks based on the calibration factor. A QVector is used to stores all the data points that the user clicks and they can also be exported as text file if it is necessary.

- Additionally, there is an automatic measuring mode where the programme will automatically collect many valid points on the graph in the range set by the user.
