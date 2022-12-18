# This code if for the purpose of connecting multiple nodemcu to a single portal and then sending data to the portal.

## The code is written in arduino ide and the nodemcu is programmed using the arduino ide.

## The code is written in such a way that it can be used for any number of nodemcu.

## This directory contains two folders:

### 1. mDNS_Web_Server

### 2. SmartDustbin

## The mDNS_Web_Server folder contains the code that needs to uploaded in nodeMCU.

### The code is written in such a way that it can be used for any number of nodemcu.

## The SmartDustbin folder contains the code for the web portal.

### The web portal is made using Html, CSS, Javascript.

### The web portal calls the api's to get the data from the nodemcu.

### The web portal is hosted on the local server.

## The code is written in a way that it prevents single point of failure.

### The code is written in such a way that if one of the nodemcu fails then the other nodemcu will take over the data collection and send it to the portal.

## Please loook for the comments in the code for better understanding.

# Thank you
