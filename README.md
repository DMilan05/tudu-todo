Egyszerű Kanban App (GTK/C)

Készítsünk egy egyszerű Kanban alkalmazást 8 fővel, egy hét alatt, C nyelven és GTK felülettel. A feladatokat három oszlopban láthatjuk: Tennivaló, Folyamatban, Kész.
1. Műszaki Alapok

    Nyelv: C (klasszikus C programozás)

    Felület: GTK (a grafikus ablakokhoz)

    Fordító: CMake vagy Makefile

2. Amit Tudnia Kell (Cél)

    Adat: Egy egyszerű Task (feladat) nevű C struktúra.

    Képernyő: Egy főablak 3 rögzített oszloppal.

    Kezelés: Gomb az új feladat hozzáadásához, és egy gomb minden kártyán a mozgatáshoz (pl. Jobbra).

    Mentés: A feladatokat elmenti és betölti egy egyszerű szöveges fájlból.

3. Feladatok 8 Gyakornoknak

A munka 8 részre van bontva, hogy mindenki egyszerre tudjon haladni.

|  | Feladat | Mit csinál? (Függőség) |
| 1 | Főablak és Váz | Beállítja a fő GTK ablakot és az alap elrendezést (3 oszlop). Betölti a stílust (Függ: 8). |
| 2 | Adat és Fájlkezelés | Létrehozza a Task adatmodellt. Megírja a feladatok mentését és betöltését fájlból. |
| 3 | Feladatkártya | Elkészíti az egy feladatot megjelenítő kis GTK komponenst (cím, leírás, Mozgatás gomb). |
| 4 | Új Feladat Ablak | Megírja a felugró GTK ablakot, ahol be lehet írni az új feladat címét és leírását. |
| 5 | Oszlop Lista | Létrehozza az újrahasználható GTK komponenst, ami tartja a feladatkártyákat (mint egy görgethető lista). Ez integrálja a 3-as munkáját. |
| 6 | Fő Logika (A Ragasztó) | Összeköti a gombnyomásokat (3, 4, 5) az adatokkal (2). Kezeli a feladatok állapotváltozásait és frissíti a listákat a képernyőn. |
| 7 | Összeállítás és Dokumentáció | Létrehozza a CMakeLists.txt (vagy Makefile) fájlt. Gondoskodik róla, hogy az egész összeálljon. |
| 8 | Vizuális Stílus (CSS) | GTK CSS stílus megírása. Ettől fog jól kinézni az app (1, 3, 4, 5). |
4. Kezdőpontok

    Közös Task Header: A 2-es és 6-os gyakornoknak azonnal el kell dönteni, hogy fog kinézni a task_model.h fájl.

    Gombok Nevei: A 6-os gyakornoknak tudnia kell, milyen neveket adnak a gomboknak és eseményeknek a 3-as és 4-es gyakornokok.

Kezdjük el a kódolást!
