Advanced Spray System for Unreal Engine

The Advanced Spray System is a highly customizable and efficient solution for creating surface-aligned sprays in Unreal Engine. Built with C++ and Blueprint integration, it supports dynamic decal placement, multiplayer synchronization, and precise surface detection using line traces.

Features

Dynamic Spray Placement: Automatically aligns sprays based on surface normals (walls, ceilings, or floors).
Randomized Textures: Supports a collection of spray textures for varied decals.
Camera-Aligned Traces: Uses the camera's forward vector to determine spray direction.
Multiplayer Ready: Fully replicated system ensures synchronized spraying across clients.
Surface Validation: Ensures sprays only appear on valid surfaces like static meshes.
Debug Tools: Includes line trace and hit point visualizations for development.
How It Works
Line Tracing: The system calculates spray placement by tracing from the player's camera forward vector to a valid surface.
Surface Detection: Validates the surface using ImpactNormal to determine orientation (e.g., walls, ceilings).
Spray Actor Spawning:
A SprayActor is spawned at the impact point.
The spray’s texture, rotation, and scale are dynamically adjusted.
Replication: Uses Server and Multicast functions to synchronize spray creation across all clients.
Core Components
1. SprayActor
Manages spray decals and applies random textures.
Includes server and multicast functions for texture synchronization.
2. SprayActorComponent
Handles spray placement logic and input binding.
Validates sprayable surfaces and manages active sprays to enforce limits.
Setup Instructions
Add the Component:

Attach SprayActorComponent to your character or pawn in the editor or via C++.
Assign a SprayActor blueprint to the SprayActorClass variable.
Input Binding:

Bind the RequestSpray function to an input action (e.g., mouse click or key press).
Configure Sprays:

Populate the SprayTextures array with your desired textures in the SprayActor blueprint or via code.
Files
SprayActor.cpp: Implements the spray decal actor logic, including random texture application and dynamic material updates​(SprayActor).
SprayActorComponent.cpp: Handles spray placement, surface validation, and actor spawning​(SprayActorComponent).
Requirements
Unreal Engine 5.0 or later.
Basic understanding of Blueprints and C++.
License
This project is licensed under the MIT License - feel free to use and modify it in your projects.
