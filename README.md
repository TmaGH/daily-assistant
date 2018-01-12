A software made with C++ and Qt aiming to have all the tools that help me have a more productive day.
Works in both Windows and Linux.

Currently has these features:
- Task Manager (tasks, task classes)
- Work timer (work class)
- Task notes (notes class)
- Saves data to local machine (serializable, binaryserializer and all classes inheriting serializable)
- Backup data to server with no ability to restore (serversync class)

Up next:
- Ability to restore data from server
- Support for multiple clients connected at the same time
- Data protection with encryption (Mandatory with data sync)
- Deadlines for tasks with changing colours as deadline approaches
- Many other features...

Server component can be found at https://github.com/TmaGH/daily-assistant-server
