Pași pentru a genera librăria dinamică ce conține algoritmii de procesare de imagine:

1. Se deschide programul CMake
2. directorul sursă este dllFactory/src
3. directorul destinatie este dllFactory/build
4. Configure
	- Specify the generator for this project: Visual Studio 15 2017 (versiunea de vs instalată pe calculatorul local)
	- Option platform for generator (if empty, generator uses: Win32): x64
	- Use default native compilers - să fie bifat
	- Finish
5. Generate
6. Open Project (proiectul s-a generat în folderul build, iar soluția este dllFactory.sln)
7. Pentru proiectele gmock, gmock_main, gtest, gtest_main trebuie făcută următoarea setare (nu am reușit să o pun în niciun fișier CMakeLists :( ):
	-> click dreapta -> Properties -> Configuration Properties -> C/C++ -> Treat Warnings As Errors -> trebuie No(/WX-)
8. se setează ca și proiect de startup dllMaker.test (acest execută eventualele teste și face build la librăriile folosite, una din ele fiind
	imageProcessing, librăria în care am implementat algoritmii de procesare).
9. se rulează cu F5
10. s-a generat fișierul dllFactory/build/bin/Debug/imageProcessingd.dll (are la sfârșitul numelui litera 'd' pt că a fost generat în urma
 rulării în debug). Acesta trebuie copiat în aplicația server din SmartCharacterScanner.