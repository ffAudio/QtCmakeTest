QtCmakeTest
===========

This is an example how to combine QT with JUCE using Cmake.

How to create Xcode projects:

```
QT_DIR=/Users/daniel/Developer/Qt/5.15.2/clang_64/lib/cmake/Qt5/
Qt5_DIR=/Users/daniel/Developer/Qt/5.15.2/clang_64/lib/cmake/Qt5/
mkdir builds
cd builds
cmake -G Xcode ..
```

And Windows:

```
set QT_DIR=C:\Qt\5.15.2-VC142-x64\5.15.2\msvc2019_64\lib\cmake\Qt5
set Qt5_DIR=C:\Qt\5.15.2-VC142-x64\5.15.2\msvc2019_64\lib\cmake\Qt5
mkdir builds
cd builds
cmake -G "Visual Studio 16" ..
```
