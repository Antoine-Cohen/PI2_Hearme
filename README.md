# PI2_Hearme
Projet PI2 Hear-me

## This is the react native app

By recording sounds with the phone microphone, the model will be able to predict it.<br>

To run the project: **react-native run-android** <br>

If you want to run the project on **ios**, be sure to include the code and permissions for ios from the node modules:<br>
[tensorflow js](https://www.npmjs.com/package/@tensorflow/tfjs-react-native)<br>
[Wave file](https://www.npmjs.com/package/wavefile)<br>
[react native fs](https://www.npmjs.com/package/react-native-fs)<br>
[ble manager](https://github.com/innoveit/react-native-ble-manager) (if you want to use Bluetooth Low Energy) <br>
[audio record](https://www.npmjs.com/package/react-native-audio-record)<br>

However the Bluetooth Low Energy connection between the ESP and the phone doesn't allow sound files transfer.

Here is a suggestion of steps to continue the project.<br> 
TODO:<br>
Classic Bluetooth: to be able to transfer sound files [maybe use this module](https://kenjdavidson.com/react-native-bluetooth-classic/) (or use Wifi module)<br>  
Speech processing and keyword extraction: to be able to summarize conversations. [Microsoft cognitive services API](https://azure.microsoft.com/fr-fr/services/cognitive-services/), [picovoice](https://www.npmjs.com/package/@picovoice/picovoice-react-native) or RNNs can be a lead (you can use other options you find as well) <br>



