import * as tf from "@tensorflow/tfjs";
import "@tensorflow/tfjs-react-native";
import { bundleResourceIO, decodeJpeg } from "@tensorflow/tfjs-react-native";
import { getFloatArray } from "../audioRecorder";

const modelJson = require("./model.json");
const modelWeights = require("./model_weights.bin");

// var wav = require('wav');

export const getModel = async () => {
  try {
    console.log("Waiting to be ready...");
    await tf.ready();
    console.log("Tensorflow ready");
    const model = await tf.loadLayersModel(
      bundleResourceIO(modelJson, modelWeights)
    );
    return model;
  } catch (error) {
    console.log("Could not load model: ", error);
  }
};

// export const get_prediction = (model) =>{

// }

export const getYamnet = async () => {
  console.log("Waiting to be ready...");
  await tf.ready();
  console.log("Tensorflow ready");

  const yamnet_Url = "https://tfhub.dev/google/tfjs-model/yamnet/tfjs/1";
  console.log("trying to load yamnet model...");
  const yamnet = await tf.loadGraphModel(yamnet_Url, {
    fromTFHub: true,
    strict: true,
  });
  return yamnet;
};

export const getEmbeddings = async (yamnet) => {
  const floatArr = await getFloatArray();
  const wav = tf.tensor(floatArr);
  // console.log(wav.shape);
  // wav.print();
  // console.log("Trying to predict...");
  const [scores, embeddings, spectrogram] = yamnet.predict(wav);
  // console.log("returning the embeddings");
  return embeddings;
};

// export const get_audio_wav_from_mic  = async () => {
//   console.log('Initiating microphone...');
//   const mic = await tf.data.microphone({
//     fftSize: 1024,
//     columnTruncateLength: 232,
//     numFramesPerSpectrogram: 43,
//     sampleRateHz:44100,
//     includeSpectrogram: false,
//     includeWaveform: true
//   });
//   console.log('Recording audio...');
//   const audioData = await mic.capture();
//   console.log('Audio recorded!');
//   const waveformTensor = audioData.waveform;
//   //waveformTensor.print();
//   mic.stop();
//   console.log('Stopped recording');

//   return waveformTensor;
// }
