# IDM Completed Downloads Cleaner

Internet Download Manager (IDM) penceresini her actiginizda tamamlanmis indirmeleri otomatik olarak temizleyen bir [Windhawk](https://windhawk.net/) modudur.

## Ne Yapar?

IDM'in ana penceresi acildiginda mod devreye girer ve su islemleri otomatik olarak gerceklestirir:

1. **Tamamlanan indirmeleri siler** - IDM'in "Tamamlanan indirmeleri sil" komutunu tetikler
2. **Onay kutusunu otomatik gecer** - Cikan onay diyalogunu gizler ve "Evet" butonuna basar
3. **Yalnizca ana pencereyi takip eder** - Ayarlar, indirme kutusu gibi diger IDM pencerelerini gormezden gelir

Tum bu islemler arka planda, milisaniyeler icinde gerceklesir.

## Nasil Calisir?

Mod, Windows API'deki `ShowWindow` fonksiyonunu hooklar. IDM bir pencereyi gorunur yaptiginda:

- Pencerenin sinif adinin `#32770` (standart dialog sinifi) olup olmadigini kontrol eder
- Pencerenin **sahipsiz** (owner yok) ve **menu cubugu olan** bir pencere olup olmadigini dogrular
- Bu iki kosulu yalnizca IDM ana penceresi karsiladigi icin diger pencereler filtrelenir
- Kosullar saglanirsa temizlik islemini ayri bir thread'de baslatir

## Kurulum

1. [Windhawk](https://windhawk.net/)'i indirip kurun
2. Windhawk'ta **"New mod"** (Yeni mod) butonuna tiklayin
3. `idm-completed-downloads-cleaner.wh.cpp` dosyasinin icerigini kod alanina yapiştirin
4. **"Compile"** (Derle) butonuna tiklayin
5. Modu etkinlestirin

## Gereksinimler

- [Windhawk](https://windhawk.net/)
- [Internet Download Manager (IDM)](https://www.internetdownloadmanager.com/)
- Windows isletim sistemi

## Notlar

- Mod yalnizca `idman.exe` isleminde calisir
- Onay kutusu basligi Turkce olarak ayarlanmistir (`Indirmelerin silinmesini onayla`)
- Farkli dilde IDM kullananlar `POPUP_TITLE` degerini kendi dillerine gore degistirmelidir

## Lisans

Bu proje acik kaynaklidir. Istediginiz gibi kullanabilir ve degistirebilirsiniz.
