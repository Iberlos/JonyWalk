# Prototype Overview

## Initial Objectives and Approach

The core mechanic behind this prototype was to explore skating movement as the primary gameplay mechanic. From the start, I focused on extending Unreal Engine’s CharacterMovementComponent by implementing a custom MovementMode tailored to skating behavior.

Development initially began in UE5.3, but engine-related issues led me to restart the project in UE5.7. This restart helped clarify the intended direction and structure of the skating system.

The movement system was designed around linear, constant forward motion, with acceleration triggered through animation notifies. Additional factors such as slope influence and drag were incorporated to create a more dynamic feel, while still allowing standard actions like jumping to function correctly. The goal was to achieve a basic movement style reminiscent of classic skating games such as Tony Hawk’s Pro Skater.

## Implementation Notes

A significant part of the work involved modifying the movement component to support switching between MovementModes. While the core movement behavior was successfully implemented, issues with velocity continuity—especially during jumps—proved challenging.

Another major issue arose from physics interactions. Because the movement system did not fully align with Unreal’s physics update pipeline, overlap events became unreliable. This directly impacted the scoring system, which depended on trigger overlaps with obstacles.

Obstacles themselves were designed to be flexible. They dynamically adapt to any roughly rectangular static mesh, automatically configuring collision and trigger volumes.

## Conclusion

Although the prototype did not meet its goals, it provided valuable experience working with Unreal Engine systems outside my usual scope. In previous roles, I focused primarily on backend mechanics while collaborating with artists who handled animation and integration. This project required a broader, more independent approach.

In hindsight, the scope was too ambitious for the available time. A more practical approach might have been to build on an existing system, such as a vehicle movement component, to accelerate development of the core mechanics allowing for more time for tasks I was not so familiar with.