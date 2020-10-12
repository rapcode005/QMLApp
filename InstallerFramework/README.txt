To build the installer, open a command prompt.

You must have the Qt installer framework installed.

cd to the NAVS directory that has the two folders "config" and "packages"

execute the command:

C:\Qt\QtIFW2.0.5\bin\repogen -p packages repository

This will build the repository

Thern create the installer by executing the foillowing command:

C:\Qt\QtIFW2.0.5\bin\repogen -p packages repository

You can test the installer locally by changing your hosts file so app.navsnow.com points to your machine.  Add this to the file C:\Windows\System32\drivers\etc\hosts

127.0.0.1	app.navsnow.com

Then in a command prompt, execute this command:

python -m http.server 80

Now double click the installer.exe