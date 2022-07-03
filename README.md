Practice project that's not in development.

A software made with C++ and Qt aiming to have all the tools needed for the most productive day.
Works in both Windows and Linux.

Currently has these features:
- Task Manager (tasks, task classes)
- Work timer (work class)
- Task notes (notes class)
- Saves data to local machine (serializable, binaryserializer and all classes inheriting serializable)
- Backup and restore function with server for one client (serversync class)

Up next:
- Backup and restore for multiple clients connected at the same time
- Data protection with encryption (Mandatory with data sync)
- Deadlines for tasks with changing colours as deadline approaches
- Many other features...

Server component can be found at https://github.com/TmaGH/daily-assistant-server
