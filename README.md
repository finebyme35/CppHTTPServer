# CppHTTPServer
Http Server isteği nasıl gidiyor socketden nasıl istek atılıyor.
IPv4 kullanırken kendi ip miz yerine localhost kullanımı
Socket Ne zaman ne zaman create ediliyor ne zaman bind yapılıp listen komutu ile dinleniyor.
Recv ile mesaj alınıp send ile geri döndürülüyor ne cevap alınmış o iletiliyor.
TCP mi UDP mi ne kullanacağımız seçiliyor 

UDP Hızlı bağlantısız güvenilir olmayan. Video streaming, oyun
Paket kaybolursa sıra bozulursa bakmaz.


TCP Güvenilir sıralı bağlantı tabanlı http, dosya transferi email. Her byte önemli.
Paket kaybolursa tekrarlama sıra bozulursa düzeltme.

HTTP SERVER Her byte önemli olduğunda TCP ile devam edilir.