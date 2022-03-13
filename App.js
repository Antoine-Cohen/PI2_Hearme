import { StatusBar } from "expo-status-bar";
import React, { Component, useRef, useState } from "react";
import { View, Text, StyleSheet, AppRegistry, Pressable } from "react-native";
import * as tf from "@tensorflow/tfjs";
import { getModel, getYamnet, getEmbeddings } from "./model/model";
import { startRecording, stopRecording } from "./audioRecorder";

var rnfs = require("react-native-fs");

class App extends React.Component {
  //const [isProcessing, setIsProcessing] = useState(false);
  //Load the model

  constructor(props) {
    super(props);

    this.state = {
      yamnet: null,
      model: null,
      predicted_class: "No sound detected",
      waveform_shape: "No shape",
    };
  }

  async componentDidMount() {
    console.log("Component mounted");
    let yamnet_ = await getYamnet();
    let model_ = await getModel();
    this.setState({ yamnet: yamnet_, model: model_ }, () => {
      console.log("Loaded model and yamnet!");
    });
  }

  componentDidUpdate() {
    console.log("Component updated!");
  }

  async updateClass() {
    console.log("getting results...");
    const embeddings = await getEmbeddings(this.state.yamnet);
    const result_tensor = this.state.model.predict(embeddings);
    const sounds = [
      "dog",
      "cat",
      "vacuum_cleaner",
      "thunderstorm",
      "washing-machine",
      "car_horn",
      "alarm_clock",
      "train",
      "airplane",
      "coughing",
    ];
    const index = tf.mean(result_tensor, 0).argMax(0).arraySync();
    let inferred_class = sounds[index];
    console.log("Class inferred:", inferred_class);
    this.setState({ predicted_class: inferred_class }, () => {
      console.log("Inferred class state updated");
    });
  }

  render() {
    return (
      <View style={styles.container}>
        <Text>AudioApp</Text>
        <Pressable style={styles.loadModel} onPress={() => this.updateClass()}>
          <Text>Predict sound</Text>
        </Pressable>
        <Text>Inferred Class: {this.state.predicted_class}</Text>
        <Pressable
          style={styles.loadModel}
          onPress={() => {
            console.log("pressed on record button!");
            startRecording();
          }}
        >
          <Text>Record audio</Text>
        </Pressable>
        <Text>Waveform shape: {this.state.waveform_shape}</Text>

        <Pressable style={styles.loadModel} onPress={() => stopRecording()}>
          <Text>Stop Recording</Text>
        </Pressable>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#fff",
    alignItems: "center",
    justifyContent: "center",
  },
  loadModel: {
    width: 150,
    height: 50,
    marginTop: 60,
    borderRadius: 24,
    color: "white",
    alignItems: "center",
    justifyContent: "center",
    backgroundColor: "red",
  },
});

export default App;
