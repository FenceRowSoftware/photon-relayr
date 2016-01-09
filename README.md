# photon-relayr

This code will allow you to use your Particle Photon to read temperature and humidity data from a DHT11 sensor and transmit the data using MQTT, via the Relayr cloud.

# Add Your Photon to Relayr

You will need to add your device to the Relayr dashboard to get the credentials required to connect to the MQTT channel for this app.  [This page](https://blog.relayr.io/creating-prototypes-on-our-dashboard/) and [this page](https://github.com/relayr/relayr-photon) provide a good overview of adding your Photon as a prototype in Relayr.

#Program Your Photon

Use this code to program your Photon.  You will need the credentials from Relayr as well as the device ID of your Photon.  Be sure to include the libraries referenced at the top of the file!  Finally, verify and flash your code and you're good to go.

You can either monitor your device output through the Particle Dashboard [here](https://dashboard.particle.io/user/logs) or on Relayr [here](https://developer.relayr.io/dashboard/devices) or build a web client to log or display the device readings.
