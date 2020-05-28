# AskAnswerQuiz
Quiz in console written on C++

//////////////////////////////////////////////////////////////

// Programm works on x86

// If you use my save, then to get access to the admin control, enter the following: login - testadmin | password - 343049
// In administrator window, you can change this data to your convenience

//////////////////////////////////////////////////////////////

//? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? Description of the mechanics of the program? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

// The first thing the interface will do is load data about all users (without statistics)

// The second thing the interface will do is it allows you to register a user, log in to an already created account and continue working with him.

// The third thing the interface will do is transfer control to the user or administrator.
// From here, tests will be carried out, statistics will be reviewed and the administration will work.

///////////////////////////////////// File communication /////////// //////////////////////////

// The program works with the "Data" folder.
// Inside the "Data" there are two folders: "Sections", "User Data"

// In the last programm stores user information. UserData.bin contains encrypted logics, passwords, and user IDs.
// Identification numbers allow users to link to their additional files (for example: 34295.bin) in which statistic is stored.

// In the "Sections" folder, two files are stored, one "Sections.bin" and the "Tests" folder. The binary list stores section names and IDs.
// binary file file, which is stored in the “Tests” folder, in which there is a test sheet (name of the test, test ID), which is associated with the section from “Sections.bin”.
// Test sheets in turn participate in a file that stores the tests themselves for passing.        
