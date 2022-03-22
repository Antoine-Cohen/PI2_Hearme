// import { BleManager, Device } from "react-native-ble-plx";
// import { PermissionsAndroid } from "react-native";

// const manager = new BleManager();

// const getBlePermissions = async () => {
//   const granted = await PermissionsAndroid.requestMultiple(
//     [
//       PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
//       PermissionsAndroid.PERMISSIONS.BLUETOOTH_ADVERTISE,
//       PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
//     ],
//     {
//       title: "Ask Permission",
//       message: "Allow Hear-me to access bluetooth?",
//       buttonPositive: "OK",
//       buttonNegative: "No",
//       buttonNeutral: "Ask me later",
//     }
//   );

//   return granted;
// };
// export const startBle = async () => {
//   await BleManager.start({ showAlert: false });
//   console.log("Module initialized");
// };
