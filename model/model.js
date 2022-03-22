import * as tf from "@tensorflow/tfjs";
import "@tensorflow/tfjs-react-native";
import { bundleResourceIO, decodeJpeg } from "@tensorflow/tfjs-react-native";
import { getFloatArray } from "../audioRecorder";

const modelJson = require("./better_model.json");
const modelWeights = require("./better_weights.bin");

// retrieve the model
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

// Request the yamnet model from tensorflow Hub
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

// Make a prediction with yamnet and extract the embeddings
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
