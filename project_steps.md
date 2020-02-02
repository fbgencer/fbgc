# Project Steps

1. Parser ve Grammar tablosu oluşturma otomatik hâle getirilecek

   ​	Semboller yazılırken önce c fonksiyonu mu değil mi diye bakılıyor. Bunu değiştirmek gerek.

- [ ] c-fonksiyonlarını önceden tanıyoruz. Böylece kullanıcı yeni bir fonksiyon tanımlayıp c fonks 

  dokunamıyor.



# TODO

- [ ] x = 1,2,3 şeklinde girdileri doğrudan tuple olarak anlamalıyız

  Bu durumda gramer tablosu düzeltilince yapılması gereken testler var

  x = print(1,2,3,5,6)

  x = print 1,2,3,5,6

  yukarıdakilerin her ikisi de fonksiyon çağırmada çalışıyor

  x = y 1,2,3,5,6 bu durumda  [M]->{1}ID{G<1>}{2}{3}{5}{6}{BUILD_TUPLE(5)}{=}<->[T] şeklinde çıktı alıyoruz. Identifier unbalanced liste görünce çıkmaması gerekiyor.

- [ ] load fonksiyonu c kütüphanelerini taramak için kullanılmalı

  - bazı kütüphaneler io,math gibi default olarak import edilsin.

- [ ] hatalar anlamlı bir biçimde ekrana bastırılsın

