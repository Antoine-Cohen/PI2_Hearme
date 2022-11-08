import { StatusBar } from "expo-status-bar";
import React, { Component, useRef, useState } from "react";
import {
  View,
  Text,
  StyleSheet,
  AppRegistry,
  Pressable,
  ActivityIndicator,
} from "react-native";
import * as tf from "@tensorflow/tfjs";
import { getModel, getYamnet, getEmbeddings } from "./model/model";
import { startRecording, stopRecording } from "./audioRecorder";
// import { startBle } from "./Bluetooth";

// Commentaire ajouté
var rnfs = require("react-native-fs");

// Main component
class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      yamnet: null, // yamnet model
      model: null, // our model
      predicted_class: "No sound detected",
      recording: "Not recording",
      score: "No score",
      loading: true,
    };
  }
  // Load both models once the app starts
  async componentDidMount() {
    console.log("Component mounted");
    let yamnet_ = await getYamnet();
    let model_ = await getModel();
    // await startBle();
    this.setState({ yamnet: yamnet_, model: model_, loading: false }, () => {
      console.log("Loaded model and yamnet!");
    });
  }

  // Function to predict a sound and calculate the score (accuracy)
  async updateClass() {
    this.setState({ loading: true });
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
    const max = tf.mean(result_tensor, 0).max().arraySync();
    const sum = tf.mean(result_tensor, 0).sum().arraySync();
    const score = max / sum;
    let inferred_class = sounds[index];
    console.log("Class inferred:", inferred_class);
    this.setState(
      { predicted_class: inferred_class, loading: false, score: score },
      () => {
        console.log("Inferred class state updated");
      }
    );
  }

  render() {
    if (!this.state.loading) {
      return (
        <View style={styles.container}>
          <Text>AudioApp</Text>
          <Pressable
            style={styles.loadModel}
            onPress={() => this.updateClass()}
          >
            <Text>Predict sound</Text>
          </Pressable>
          <Text>Inferred Class: {this.state.predicted_class}</Text>
          <Text>Score: {this.state.score}</Text>
          <Pressable
            style={styles.loadModel}
            onPress={() => {
              console.log("pressed on record button!");
              this.setState({ recording: "Recording" });
              startRecording();
            }}
          >
            <Text>Record audio</Text>
          </Pressable>
          <Text>{this.state.recording}</Text>

          <Pressable
            style={styles.loadModel}
            onPress={() => {
              this.setState({ recording: "Not recording" });
              stopRecording();
            }}
          >
            <Text>Stop Recording</Text>
          </Pressable>
        </View>
      );
    } else {
      return (
        <View style={styles.container}>
          <ActivityIndicator size="large" color="#00ff00" />
        </View>
      );
    }
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
