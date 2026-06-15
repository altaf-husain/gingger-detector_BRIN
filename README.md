<h1>Gingerol and Shogaol Detection System for <i>Jahe Emprit</i></h1>

<p>
This project is a portable smart sensing system designed to estimate the
<b>gingerol</b> and <b>shogaol</b> content of <i>Jahe Emprit</i> using a
<b>SparkFun AS7265x multispectral sensor</b> and a <b>Machine Learning</b>
model deployed on an <b>Arduino Nano 33 BLE</b>.
</p>

<p>
The system captures spectral responses at <b>18 wavelengths</b> covering
ultraviolet (UV), visible (VIS), and near-infrared (NIR) regions. The spectral
data are processed by a Random Forest regression model trained using laboratory
reference measurements of gingerol and shogaol concentrations. The final model
runs directly on the microcontroller, enabling rapid and non-destructive quality
assessment without laboratory testing.
</p>

<h2>Arduino Nano 33 BLE</h2>

<p>
The Arduino Nano 33 BLE was selected because it provides significantly more
processing power and memory than traditional Arduino boards such as the Arduino
Uno or Nano. Random Forest models require sufficient flash memory and RAM to
store decision trees and perform real-time predictions.
</p>

<p>
Arduino Nano BLE 33 Datasheet:
<a href="https://github.com/eloquentarduino/micromlgen" target="_blank">
https://share.google/I0FFK0FqzV9vJVGZK
</a>
</p>

<p>
  Install the Board Package :
  <ol>
    <li>Open the Arduino IDE.</li>
    <li>Click the Board Manager icon on the left sidebar (or go to Tools > Board > Boards Manager).</li>
    <li>Search for "mbed" in the search bar.</li>
    <li>Find Arduino Mbed OS Nano Boards and click Install.</li>
    <li>Restart the Arduino IDE once the installation finishes.</li>
  </ol>
</p>

<p>
The board is powered by a 32-bit ARM Cortex-M4 processor, which allows faster
execution of machine learning algorithms while maintaining low power
consumption. In addition, the built-in Bluetooth Low Energy (BLE) module
provides opportunities for future wireless monitoring and data communication.
</p>

<h2>Machine Learning Development Process</h2>

<p>
The machine learning model was developed using a dataset consisting of
laboratory measurements and spectral sensor readings.
</p>

<p>
The first column of the dataset contains the laboratory analysis results,
which serve as the target variable (<b>Y</b>). The remaining columns contain
the spectral data obtained from the AS7265x sensor, representing
<b>18 wavelength features</b> used as input variables (<b>X</b>).
</p>

<p>
Before model training, the dataset was cleaned and organized to ensure data
consistency. The spectral features were then separated from the laboratory
reference values and prepared for machine learning analysis.
</p>

<p>
A <b>Random Forest Regressor</b> was used to model the relationship between
spectral responses and gingerol/shogaol concentrations. Hyperparameter tuning
was performed to determine the optimal values for:
</p>

<ul>
    <li><code>max_depth</code></li>
    <li><code>max_features</code></li>
    <li><code>min_samples_leaf</code></li>
    <li><code>n_estimators</code></li>
</ul>

<p>
The training and validation process was repeated until the model achieved a
minimum validation performance of <b>R² ≥ 0.70</b>, indicating acceptable
prediction accuracy.
</p>

<h2>Converting the Model for Arduino Deployment</h2>

<p>
After obtaining the best-performing Random Forest model, the trained model was
converted into C/C++ code using the <b>micromlgen</b> library.
</p>

<p>
GitHub Repository:
<a href="https://github.com/eloquentarduino/micromlgen" target="_blank">
https://github.com/eloquentarduino/micromlgen
</a>
</p>

<p>
The micromlgen library converts the optimized Random Forest model, including
its hyperparameters, into a lightweight C/C++ header file. This generated code
can be directly integrated into an Arduino project as a custom machine learning
library.
</p>

<p>
Once uploaded to the Arduino Nano 33 BLE, the device reads the 18 spectral
features from the AS7265x sensor and performs real-time prediction of gingerol
and shogaol content without requiring a computer or internet connection.
</p>


<h2>Project Contribution</h2>

<p>
This project demonstrates the integration of multispectral sensing and embedded
machine learning to create a portable, low-cost, and non-destructive system for
estimating the chemical quality of agricultural products. The approach reduces
the need for laboratory testing and enables rapid field-based quality assessment
of ginger samples.
</p>
