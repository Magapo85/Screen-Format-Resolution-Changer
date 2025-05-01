# Screen-Format-Resolution-Changer

A quick and easy way to change the format resolution of your computer's main screen between 16/9 and 4/3.

Compile this cpp code and run the .exe file using one of the following three methods :
1) Open ResChange.exe to see and use the UI
2) In the terminal or command prompt to set 1920x1080 (16/9) => ResChange.exe 16/9
3) In the terminal or command prompt to set 1024x768 (4/3) => ResChange.exe 4/3

You can modifiy (2 lines in the code both format) as your computer possibilites :

-> line 52 and 139 : 1920x1080 (16/9) by 3840x2160 or 1280x720 or 854x480 or other ...

-> line 55 and 141 : 1024x768 (4/3) by 1280x960 or 800x600 or 960x720 or 1440x1080 or other ...

=> https://en.wikipedia.org/wiki/Display_resolution_standards

Test on Windows environment (w10 and w11)

Note : if it's failed, maybe checked screen frequency rate, change the value line 169
