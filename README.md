# iFollow

limitations : timers must be addeded in an specific order (high->low) because the implementation of triggerAlarm when two or more alarms are activated and the higher one is stopped, we immediatly jump to the next one (inside the `alarms` vector ) to check if it is activated and started it.