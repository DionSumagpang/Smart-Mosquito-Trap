# Smart Mosquito Trap

"""Overview

Smart Mosquito Trap is a social innovation project formally titled "Development of a Low-Power Light-and-Fan Mosquito Trap with Integrated Risk Index and Data Logging System for Dengue Prevention." The project aims to contribute to dengue prevention in the Philippines by providing an energy-efficient and intelligent mosquito trapping system.

The prototype utilizes a UV light and a low-power DC fan as its primary mosquito attract-and-capture mechanism. An optional titanium dioxide (TiO₂) coating may also be incorporated to enhance mosquito attraction by simulating human breath when activated under ultraviolet light.

To improve energy efficiency, the system integrates a DHT11 temperature and humidity sensor to estimate periods of increased mosquito activity based on findings from related studies. These environmental readings are used to calculate a simple mosquito risk index, allowing the system to automatically adjust the fan speed while maintaining continuous UV illumination. This adaptive operation helps reduce unnecessary power consumption during periods of lower mosquito activity.

The project also features a dual data logging system. Sensor readings and mosquito counts are transmitted to a Blynk IoT dashboard whenever an internet connection is available. In the absence of connectivity, the system continues operating normally by storing all collected data locally on a microSD card, ensuring that no information is lost and can be reviewed later.

Note: This repository contains the firmware developed for a low-fidelity prototype. The code is intended to demonstrate the system's core functionalities, including mosquito detection, adaptive fan control, environmental monitoring, local data logging, and IoT connectivity. It does not represent the complete implementation of all planned features described in the study."""
