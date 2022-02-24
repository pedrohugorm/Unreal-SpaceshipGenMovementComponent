# UNREAL Spaceship Movement Component
An UNREAL Implementation of a Network Replicated/Predicted Spaceship Movement Component

Implemented using the Plugin: [General Movement Component](https://marketplace-website-node-launcher-prod.ol.epicgames.com/ue/marketplace/en-US/product/general-movement-component)

Video:
[YoutubeLink](https://www.youtube.com/watch?v=UrenweCyPng)

## Usage
* Replace the Plugin API Macro RTSFPS_API with your Games' or Plugin's.
* Paste the files in their respective Public/Private Folders.
* Add `"GeneralMovement"` to your `PublicDependencyModuleNames` in your `Build.cs` file
* On the Movement Component, Section: General Movemnet Component | Networking
  * Set Network Preset as Custom
  * In the Section: General Movemnet Component | Networking | Replication | Autonomous Proxy
    * Set Replicate Rotation Pitch, Replicate Rotation Roll = True
  * In the Section: General Movemnet Component | Networking | Replication | Simulated Proxy
    * Set Replicate Rotation Pitch, Replicate Rotation Roll = True
  * In the Section: General Movemnet Component | Networking
    * Set Velocity Error = 5.0

## TODOs
* Make this a Plugin for easier integration
* Add Demo HUD
* Add a Better Video
