import AudioRecord from "react-native-audio-record";
import { PermissionsAndroid } from "react-native";
import { Buffer } from "buffer";
import { WaveFile } from "wavefile";

let audio_path = "";
var RNFS = require("react-native-fs");
let wav = new WaveFile();

const getPermissions = async () => {
  try {
    const granted = await PermissionsAndroid.request(
      PermissionsAndroid.PERMISSIONS.RECORD_AUDIO,
      {
        title: "Ask Permission",
        message: "I would like to access the microphone please",
        buttonPositive: "OK",
        buttonNegative: "No",
        buttonNeutral: "Ask me later",
      }
    );
    console.log("Permission:", granted);
    return granted;
  } catch (err) {
    console.error(err);
  }
};

export const startRecording = async () => {
  base64_data = [];
  const permission = await getPermissions();
  const options = {
    sampleRate: 16000, // default 44100
    channels: 1, // 1 or 2, default 1
    bitsPerSample: 16, // 8 or 16, default 16
    audioSource: 6, // android only (see below)
    wavFile: "test.wav", // default 'audio.wav'
  };

  AudioRecord.init(options);
  console.log("Starting Recording!");
  AudioRecord.start();
  console.log("Started recording!");
};

export const stopRecording = () => {
  AudioRecord.stop();
  console.log("Stopped recording!");
};

export const getFloatArray = async () => {
  try {
    audio_path = RNFS.DocumentDirectoryPath + "/test.wav";

    let exists = await RNFS.exists(audio_path);
    console.log(`Exists: ${exists}`);
    if (exists == false) {
      console.log("File does not exist");
      return null;
    } else {
      console.log("File exists!");

      const file_contents = await RNFS.readFile(audio_path, "base64");
      wav.fromBase64(file_contents);
      let samples = wav.getSamples(false, Int16Array);
      let s = [];

      for (var i = 0, length = samples.length; i < length; i++) {
        s.push(samples[i] / 32768);
      }
      // console.log(s);
      return s;
    }
  } catch (err) {
    console.error(err);
  }
};
